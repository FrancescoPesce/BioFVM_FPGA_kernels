#include "forward_to_backward.hpp"
#include "defines.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t line_length = 20;
    const uint32_t num_lines_per_group = 7;
    const uint32_t N = line_length * num_lines_per_group * GROUP_SIZE;

    ap_uint<64> input[N];
    for (uint32_t i = 0; i < N; i++) {
        input[i] = i;
    }

    hls::stream<ap_uint<64>> input_stream;
    for (uint32_t i = 0; i < N; i++) {
        input_stream.write(input[i]);
    }

    // Create correct (golden) outputs.
    ap_uint<64> golden_output[N];
    for (uint32_t i = 0; i < num_lines_per_group; i++) {
        for (uint32_t j = 0; j < line_length; j++) {
            for (uint32_t k = 0; k < GROUP_SIZE; k++) {
                golden_output[i * line_length * GROUP_SIZE + j * GROUP_SIZE + k] = input[i * line_length * GROUP_SIZE + (line_length-j-1) * GROUP_SIZE + k];
            }
        }
    }

    hls::stream<ap_uint<64>> output_stream;

    // Run the kernel as a C++ function.
    forward_to_backward(input_stream, output_stream, line_length, num_lines_per_group);

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