#include "naive_stream2mem.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t N = 1024;

    ap_uint<64> input[N];
    for (uint32_t i = 0; i < N; i++) {
        input[i] = i * i;
    }

    // Create correct (golden) outputs.
    ap_uint<64> golden_output[N];
    for (uint32_t i = 0; i < N; i++) {
        golden_output[i] = input[i];
    }

    hls::stream<ap_uint<64>> input_stream;
    for (uint32_t i = 0; i < N; i++) {
        input_stream.write(input[i]);
    }

    ap_uint<64> output[N];

    // Run the kernel as a C++ function.
    naive_stream2mem(output, input_stream, N);

    // Check the results.
    bool pass = true;
    for (uint32_t i = 0; i < N; i++) {
        if (output[i] != golden_output[i]) {
            std::cout << "Mismatch at index " << i << ": expected " << golden_output[i] << ", got " << output[i] << std::endl;
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