#include "pe_loopback.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t line_length = 10;
    const uint32_t num_lines_per_group = 17;
    const uint32_t N = num_lines_per_group * GROUP_SIZE * line_length;

    ap_uint<REAL_WIDTH> density_pe[N];
    for (uint32_t i = 0; i < N; i++) {
        density_pe[i] = i * i;
    }

    hls::stream<ap_uint<REAL_WIDTH>> from_pe_x3;
    for (uint32_t i = 0; i < N; i++) {
        from_pe_x3.write(density_pe[i]);
    }

    // Create correct (golden) outputs.
    ap_uint<REAL_WIDTH> golden_loopback[N];
    int addr = 0;
    for (uint32_t i = 0; i < num_lines_per_group; i++) {
        for (uint32_t k = 0; k < GROUP_SIZE; k++) {
            golden_loopback[addr] = 0;
            addr++;
        }

        for (uint32_t j = 0; j < line_length-1; j++) {
            for (uint32_t k = 0; k < GROUP_SIZE; k++) {
                golden_loopback[addr] = density_pe[i*GROUP_SIZE*line_length + j*GROUP_SIZE + k];
                addr++;
            }
        }
    }
    ap_uint<REAL_WIDTH> golden_output[N];
    memcpy(golden_output, density_pe, sizeof(ap_uint<REAL_WIDTH>)*N);

    hls::stream<ap_uint<REAL_WIDTH>> density_loopback;
    hls::stream<ap_uint<REAL_WIDTH>> density_output;

    // Run the kernel as a C++ function.
    pe_loopback(from_pe_x3, density_loopback, density_output, line_length, num_lines_per_group);

    // Read the outputs from the stream.
    ap_uint<REAL_WIDTH> loopback[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        loopback[words_read] = density_loopback.read();
    }
    ap_uint<REAL_WIDTH> output[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        output[words_read] = density_output.read();
    }

    // Check the results.
    bool pass = true;
    for (uint32_t i = 0; i < N; i++) {
        if (loopback[i] != golden_loopback[i]) {
            std::cout << "loopback: Mismatch at index " << i << ": expected " << golden_loopback[i] << ", got " << loopback[i] << std::endl;
            pass = false;
        }
        if (output[i] != golden_output[i]) {
            std::cout << "output: Mismatch at index " << i << ": expected " << golden_output[i] << ", got " << output[i] << std::endl;
            pass = false;
        }
    }

    if (pass) {
        std::cout << "Test passed!" << std::endl;
    } else {
        std::cout << "Test failed!" << std::endl;
    }

    return 0;
}