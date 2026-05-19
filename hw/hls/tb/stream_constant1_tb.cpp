#include "stream_constant1.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t NS = 5;
    const uint32_t NV = 10;
    const uint32_t N = NS * NV;

    ap_uint<64> input[NS];
    for (uint32_t i = 0; i < NS; i++) {
        input[i] = i * i;
    }

    // Create correct (golden) outputs.
    ap_uint<64> golden_output[N];
    for (uint32_t i = 0; i < NS; i++) {
        for (uint32_t j = 0; j < NV; j++) {
            golden_output[i * NV + j] = input[i];
        }
    }

    hls::stream<ap_uint<64>> output_stream;

    // Run the kernel as a C++ function.
    stream_constant1(input, output_stream, NV, NS);

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