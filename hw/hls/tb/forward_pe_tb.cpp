#include "forward_pe.hpp"
#include "defines.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t N = 100;

    double x1_array[N];
    double x2_array[N];
    double c1_array[N];
    double c2_array[N];
    for (uint32_t i = 0; i < N; i++) {
        x1_array[i] = i;
        x2_array[i] = i * 2;
        c1_array[i] = i * 3;
        c2_array[i] = i * 4;
    }

    hls::stream<ap_uint<64>> x1;
    hls::stream<ap_uint<64>> x2;
    hls::stream<ap_uint<64>> c1;
    hls::stream<ap_uint<64>> c2;
    for (uint32_t i = 0; i < N; i++) {
        x1.write(*reinterpret_cast<ap_uint<64>*>(&(x1_array[i])));
        x2.write(*reinterpret_cast<ap_uint<64>*>(&(x2_array[i])));
        c1.write(*reinterpret_cast<ap_uint<64>*>(&(c1_array[i])));
        c2.write(*reinterpret_cast<ap_uint<64>*>(&(c2_array[i])));
    }

    // Create correct (golden) outputs.
    double golden_output[N];
    for (uint32_t i = 0; i < N; i++) {
        golden_output[i] = (x1_array[i] * c1_array[i] + x2_array[i]) * c2_array[i];
    }

    hls::stream<ap_uint<64>> x3;

    // Run the kernel as a C++ function.
    forward_pe(x1, x2, c1, c2, x3, N);

    // Read the outputs from the stream.
    double output[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        ap_uint<64> temp = x3.read();
        output[words_read] = *reinterpret_cast<double*>(&temp);
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