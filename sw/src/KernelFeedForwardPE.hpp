#ifndef KERNEL_FEED_FORWARD_PE_HPP
#define KERNEL_FEED_FORWARD_PE_HPP

#include <string>
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"

class KernelFeedForwardPE {
    public:
        // Create XRT objects for device, kernel and run.
        KernelFeedForwardPE(xrt::device device, xrt::uuid xclbin_uuid)
            : device(device) {
            // Create kernel object from the xclbin.
            krnl = xrt::kernel(device, xclbin_uuid, kernel_name.c_str());

            // Create a run object for the kernel.
            run = xrt::run(krnl);
        }

        // Set kernel arguments.
        void set_inputs(int line_length, int num_lines_per_group) {
            // Set kernel arguments.
            run.set_arg(arg_line_length, line_length);
            run.set_arg(arg_num_lines_per_group, num_lines_per_group);
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
        inline static const std::string kernel_name = "feed_forward_pe";

        // Indices of kernel arguments, must match the order in hw.
        static constexpr int arg_line_length         = 4;
        static constexpr int arg_num_lines_per_group = 5;

        // XRT objects for device, kernel, buffers, and run.
        xrt::device device;
        xrt::kernel krnl;
        xrt::bo buf_out;
        xrt::run run;
};

#endif // KERNEL_FEED_FORWARD_PE_HPP