#ifndef KERNEL_STREAM_CONSTANT1_HPP
#define KERNEL_STREAM_CONSTANT1_HPP

#include <string>
#include "ap_int.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"

class KernelStreamConstant1 {
    public:
        // Create XRT objects for device, kernel and run.
        KernelStreamConstant1(xrt::device device, xrt::uuid xclbin_uuid)
            : device(device) {
            // Create kernel object from the xclbin.
            krnl = xrt::kernel(device, xclbin_uuid, kernel_name.c_str());

            // Create a run object for the kernel.
            run = xrt::run(krnl);
        }

        // Set kernel arguments and write input data to the device.
        void set_inputs(ap_uint<64>* input, int NV, int NS) {
            // Set banks for the input buffer.
            xrtMemoryGroup bank_input = krnl.group_id(arg_input);

            // Create the input buffer.
            buf_in = xrt::bo(device, NS * 64 / 8, xrt::bo::flags::normal, bank_input);
            
            // Set kernel arguments.
            run.set_arg(arg_input, buf_in);
            run.set_arg(arg_NV, NV);
            run.set_arg(arg_NS, NS);

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
        inline static const std::string kernel_name = "stream_constant1";

        // Indices of kernel arguments, must match the order in hw.
        static constexpr int arg_input = 0;
        static constexpr int arg_NV    = 2;
        static constexpr int arg_NS    = 3;

        // XRT objects for device, kernel, buffers, and run.
        xrt::device device;
        xrt::kernel krnl;
        xrt::bo buf_in;
        xrt::run run;
};

#endif // KERNEL_STREAM_CONSTANT1_HPP