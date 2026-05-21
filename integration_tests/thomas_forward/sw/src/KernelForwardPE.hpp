#ifndef KERNEL_FORWARD_PE_HPP
#define KERNEL_FORWARD_PE_HPP

#include <string>
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"

class KernelForwardPE {
    public:
        // Create XRT objects for device, kernel and run.
        KernelForwardPE(xrt::device device, xrt::uuid xclbin_uuid)
            : device(device) {
            // Create kernel object from the xclbin.
            krnl = xrt::kernel(device, xclbin_uuid, kernel_name.c_str());

            // Create a run object for the kernel.
            run = xrt::run(krnl);
        }

        // Set kernel arguments.
        void set_inputs(int N) {
            // Set kernel arguments.
            run.set_arg(arg_N, N);
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
        inline static const std::string kernel_name = "forward_pe";

        // Indices of kernel arguments, must match the order in hw.
        static constexpr int arg_N = 5;

        // XRT objects for device, kernel, buffers, and run.
        xrt::device device;
        xrt::kernel krnl;
        xrt::bo buf_out;
        xrt::run run;
};

#endif // KERNEL_FORWARD_PE_HPP