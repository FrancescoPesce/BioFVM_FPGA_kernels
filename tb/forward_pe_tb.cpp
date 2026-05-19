#include "forward_pe.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t size = 8;

    double x1_array[size];
    double x2_array[size];
    double c1_array[size];
    double c2_array[size];
    for (uint32_t i = 0; i < size; i++) {
        x1_array[i] = i;
        x2_array[i] = i * 2;
        c1_array[i] = i * 3;
        c2_array[i] = i * 4;
    }

    hls::stream<double> x1;
    hls::stream<double> x2;
    hls::stream<double> c1;
    hls::stream<double> c2;
    for (uint32_t i = 0; i < size; i++) {
        x1.write(x1_array[i]);
        x2.write(x2_array[i]);
        c1.write(c1_array[i]);
        c2.write(c2_array[i]);
    }

    // Create correct (golden) outputs.
    double golden_output[size];
    for (uint32_t i = 0; i < size; i++) {
        golden_output[i] = (x1_array[i] * c1_array[i] + x2_array[i]) * c2_array[i];
    }

    hls::stream<double> x3;

    // Run the kernel as a C++ function.
    forward_pe(x1, x2, c1, c2, x3, size);

    // Read the outputs from the stream.
    double output[size];
    for (uint32_t words_read = 0; words_read < size; words_read++) {
        output[words_read] = x3.read();
    }

    // Check the results.
    bool pass = true;
    for (uint32_t i = 0; i < size; i++) {
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