#ifndef KERNEL_MEM_2_STREAM_HPP
#define KERNEL_MEM_2_STREAM_HPP

#include <string>
#include "ap_int.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"

class KernelMem2Stream {
    public:
        // Create XRT objects for device, kernel and run.
        KernelMem2Stream(xrt::device device, xrt::uuid xclbin_uuid)
            : device(device) {
            // Create kernel object from the xclbin.
            krnl = xrt::kernel(device, xclbin_uuid, kernel_name.c_str());

            // Create a run object for the kernel.
            run = xrt::run(krnl);
        }

        // Set kernel arguments and write input data to the device.
        void set_inputs(ap_uint<REAL_WIDTH>* input, int N) {
            // Set banks for the input buffer.
            xrtMemoryGroup bank_input = krnl.group_id(arg_input);

            // Create the input buffer.
            buf_in = xrt::bo(device, N * REAL_WIDTH / 8, xrt::bo::flags::normal, bank_input);
            
            // Set kernel arguments.
            run.set_arg(arg_input, buf_in);
            run.set_arg(arg_N, N);

            // Write input data to the device.
            buf_in.write(input);
            buf_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        }

        // Launch the kernel (non-blocking).
        void launch() {
            run.start();
        }

        // Wait for the kernel to finish.
        void wait_on_run() {
            run.wait();
        }

    private:
        // Name of the kernel, must match the name chosen in hw.
        inline static const std::string kernel_name = "naive_mem2stream";

        // Indices of kernel arguments, must match the order in hw.
        static constexpr int arg_input = 0;
        static constexpr int arg_N     = 2;

        // XRT objects for device, kernel, buffers, and run.
        xrt::device device;
        xrt::kernel krnl;
        xrt::bo buf_in;
        xrt::run run;
};

#endif // KERNEL_MEM_2_STREAM_HPP