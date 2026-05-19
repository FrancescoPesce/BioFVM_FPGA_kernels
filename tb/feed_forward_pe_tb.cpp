#include "feed_forward_pe.hpp"
#include "defines.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t line_length = 10;
    const uint32_t num_lines_per_group = 15;
    const uint32_t N = num_lines_per_group * GROUP_SIZE * line_length;
    const uint32_t reduced_N = num_lines_per_group * GROUP_SIZE * (line_length-1);

    double density_ddr[N];
    for (uint32_t i = 0; i < N; i++) {
        density_ddr[i] = i * i;
    }
    double density_pe[reduced_N];
    for (uint32_t i = 0; i < reduced_N; i++) {
        density_pe[i] = i * 5;
    }

    hls::stream<double> density_from_ddr;
    for (uint32_t i = 0; i < N; i++) {
        density_from_ddr.write(density_ddr[i]);
    }
    hls::stream<double> density_from_pe;
    for (uint32_t i = 0; i < reduced_N; i++) {
        density_from_pe.write(density_pe[i]);
    }

    // Create correct (golden) outputs.
    double golden_x1[N];
    int addr = 0;
    for (uint32_t i = 0; i < num_lines_per_group; i++) {
        for (uint32_t k = 0; k < GROUP_SIZE; k++) {
            golden_x1[addr] = 0.0;
            addr++;
        }

        for (uint32_t j = 1; j < line_length; j++) {
            for (uint32_t k = 0; k < GROUP_SIZE; k++) {
                golden_x1[addr] = density_pe[i*GROUP_SIZE*(line_length-1) + (j-1)*GROUP_SIZE + k];
                addr++;
            }
        }
    }
    double golden_x2[N];
    memcpy(golden_x2, density_ddr, sizeof(double)*N);

    hls::stream<double> to_pe_x1;
    hls::stream<double> to_pe_x2;

    // Run the kernel as a C++ function.
    feed_forward_pe(density_from_ddr, density_from_pe, to_pe_x1, to_pe_x2, line_length, num_lines_per_group);

    // Read the outputs from the stream.
    double x1[N];
    double x2[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        x1[words_read] = to_pe_x1.read();
        x2[words_read] = to_pe_x2.read();
    }

    // Check the results.
    bool pass = true;
    for (uint32_t i = 0; i < N; i++) {
        if (x1[i] != golden_x1[i]) {
            std::cout << "x1: Mismatch at index " << i << ": expected " << golden_x1[i] << ", got " << x1[i] << std::endl;
            pass = false;
        }
        if (x2[i] != golden_x2[i]) {
            std::cout << "x2: Mismatch at index " << i << ": expected " << golden_x2[i] << ", got " << x2[i] << std::endl;
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