#include "stream_denom.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t NS = 5;
    const uint32_t line_length = 20;
    const uint32_t num_lines_per_S_group = 5;
    const uint32_t NV = line_length * num_lines_per_S_group * GROUP_SIZE;
    const uint32_t N = NS * NV;
    const uint32_t size = NS * line_length;

    ap_uint<REAL_WIDTH> input[size];
    for (uint32_t i = 0; i < size; i++) {
        input[i] = i * i;
    }

    // Create correct (golden) outputs.
    ap_uint<REAL_WIDTH> golden_output[N];
    int flat_idx = 0;
    for (uint32_t i = 0; i < NS; i++) {
        for (uint32_t j = 0; j < num_lines_per_S_group; j++) {
            for (uint32_t k = 0; k < line_length; k++) {
                for (uint32_t l = 0; l < GROUP_SIZE; l++) {
                    golden_output[flat_idx] = input[i * line_length + k];
                    flat_idx++;
                }
            }
        }
    }

    hls::stream<ap_uint<REAL_WIDTH>> output_stream;

    // Run the kernel as a C++ function.
    stream_denom(input, output_stream, line_length, num_lines_per_S_group, NS);

    // Read the outputs from the stream.
    ap_uint<REAL_WIDTH> output[N];
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