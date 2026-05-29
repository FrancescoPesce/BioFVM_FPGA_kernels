#include "backward_pe.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t N = 100;

    real_t x1_array[N];
    real_t x2_array[N];
    real_t c_array[N];
    for (uint32_t i = 0; i < N; i++) {
        x1_array[i] = i;
        x2_array[i] = i * 2;
        c_array[i] = i * 3;
    }

    hls::stream<ap_uint<REAL_WIDTH>> x1;
    hls::stream<ap_uint<REAL_WIDTH>> x2;
    hls::stream<ap_uint<REAL_WIDTH>> c;
    for (uint32_t i = 0; i < N; i++) {
        x1.write(*reinterpret_cast<ap_uint<REAL_WIDTH>*>(&(x1_array[i])));
        x2.write(*reinterpret_cast<ap_uint<REAL_WIDTH>*>(&(x2_array[i])));
        c.write(*reinterpret_cast<ap_uint<REAL_WIDTH>*>(&(c_array[i])));
    }

    // Create correct (golden) outputs.
    real_t golden_output[N];
    for (uint32_t i = 0; i < N; i++) {
        golden_output[i] = x2_array[i] - x1_array[i] * c_array[i];
    }

    hls::stream<ap_uint<REAL_WIDTH>> x3;

    // Run the kernel as a C++ function.
    backward_pe(x1, x2, c, x3, N);

    // Read the outputs from the stream.
    real_t output[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        ap_uint<REAL_WIDTH> temp = x3.read();
        output[words_read] = *reinterpret_cast<real_t*>(&temp);
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