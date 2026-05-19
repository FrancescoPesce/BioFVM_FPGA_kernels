#include "naive_mem2stream.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t N = 100;

    ap_uint<64> input[N];
    for (uint32_t i = 0; i < N; i++) {
        input[i] = i * i;
    }

    // Create correct (golden) outputs.
    ap_uint<64> golden_output[N];
    for (uint32_t i = 0; i < N; i++) {
        golden_output[i] = input[i];
    }

    hls::stream<ap_uint<64>> output_stream;

    // Run the kernel as a C++ function.
    naive_mem2stream(input, output_stream, N);

    // Read the outputs from the stream.
    ap_uint<64> output[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        output[words_read] = output_stream.read();
    }

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