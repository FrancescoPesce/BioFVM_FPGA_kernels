#include <ap_int.h>
#include "KernelBackwardPE.hpp"
#include "KernelPELoopback.hpp"
#include "KernelMem2Stream.hpp"
#include "KernelStream2Mem.hpp"
#include "KernelStreamC.hpp"
#include "utils.hpp"
#include "../../../../hw/hls/include/defines.hpp"

int check_result(double* golden_output, double* output, int size) {
    for (int i = 0; i < size; i++) {
        if (output[i] != (golden_output[i])) {
            std::cout << "Error at index " << i
                      << ": " << output[i]
                      << " != " << golden_output[i] << std::endl;
            return EXIT_FAILURE;
        }
    }
    std::cout << "Test passed!" << std::endl;
    return EXIT_SUCCESS;
}

void compute_golden_output(double* input, double* c, double* golden_output, int line_length, int num_lines_per_S_group, int NS) {
    int base_addr = 0;
    for (uint32_t s = 0; s < NS; s++) {
        for (uint32_t i = 0; i < num_lines_per_S_group; i++) {
            for (uint32_t k = 0; k < GROUP_SIZE; k++) {
                double previous_output = 0.0;
                for (uint32_t j = 0; j < line_length; j++) {
                    int addr = base_addr + j * GROUP_SIZE + k;
                    golden_output[addr] = input[addr] - previous_output * c[s * line_length + line_length-j-1];
                    previous_output = golden_output[addr];
                }
            }
            base_addr += line_length * GROUP_SIZE;
        }
    }
}

int main(int argc, char* argv[]) {
    // 1. Initialize the device and load the xclbin.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <XCLBIN_PATH>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string xclbin_file = argv[1];

    char *env_emu = getenv("XCL_EMULATION_MODE");
    if (env_emu && std::string(env_emu) == "hw_emu") {
        std::cout << utils::bold_on << "Program running in hardware emulation mode" << utils::bold_off << std::endl;
    }
    else {
        std::cout << utils::bold_on << "Program running in hardware mode" << utils::bold_off << std::endl;
    }

    constexpr int device_id = 0;
    std::cout << "Loading bitstream (" << xclbin_file << ") on device " << device_id << "... ";
    xrt::device device = xrt::device(device_id);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);

    // 2. Set up the kernels (passthrough is free-running, so no need to do anything for it).
    std::cout << "Preparing the kernels" << std::endl;
    KernelBackwardPE backward_pe(device, xclbin_uuid);
    KernelPELoopback pe_loopback(device, xclbin_uuid);
    KernelMem2Stream mem2stream(device, xclbin_uuid);
    KernelStream2Mem stream2mem(device, xclbin_uuid);
    KernelStreamC stream_c(device, xclbin_uuid);
    
    // 3. Prepare kernel inputs.
    std::cout << "Preparing kernel inputs" << std::endl;
    const int NS = 5;
    const int num_lines_per_S_group = 7;
    const int line_length = 10;

    const int num_lines_per_S = num_lines_per_S_group * GROUP_SIZE;
    const int num_lines_per_group = num_lines_per_S_group * NS;
    const int N = num_lines_per_S * NS * line_length;
    const int NV = N / NS;

    double original_density[N];
    double c[NS * line_length];
    for (uint32_t i = 0; i < N; i++) {
        original_density[i] = i * i;
    }
    for (uint32_t i = 0; i < NS * line_length; i++) {
        c[i] = i + 1;
    }

    ap_uint<64>* original_density_uint = reinterpret_cast<ap_uint<64>*>(original_density);
    ap_uint<64>* c_uint = reinterpret_cast<ap_uint<64>*>(c);

    // 4. Compute golden output.
    double golden_density[N];
    compute_golden_output(original_density, c, golden_density, line_length, num_lines_per_S_group, NS);

    // 5. Transfer data to the kernels.
    std::cout << "Transferring data to the kernels" << std::endl;
    backward_pe.set_inputs(N);
    pe_loopback.set_inputs(line_length, num_lines_per_group);
    mem2stream.set_inputs(original_density_uint, N);
    stream2mem.set_inputs(N);
    stream_c.set_inputs(c_uint, line_length, num_lines_per_S_group, NS);

    // 6. Launch the kernels.
    std::cout << "Launching the kernels" << std::endl;
    backward_pe.launch();
    pe_loopback.launch();
    mem2stream.launch();
    stream2mem.launch();
    stream_c.launch();

    // 6. Wait for the kernels to finish.
    std::cout << "Waiting for kernel runs to finish" << std::endl;
    backward_pe.wait_on_run();
    pe_loopback.wait_on_run();
    mem2stream.wait_on_run();
    stream2mem.wait_on_run();
    stream_c.wait_on_run();
    
    // 8. Transfer results from the kernel.
    std::cout << "Transferring results from the kernels" << std::endl;
    double intermediate_density[N];
    ap_uint<64>* intermediate_density_uint = reinterpret_cast<ap_uint<64>*>(intermediate_density);
    stream2mem.get_outputs(intermediate_density_uint);

    // 9. Check the results.
    std::cout << "Checking results" << std::endl;
    return check_result(golden_density, intermediate_density, N);
}