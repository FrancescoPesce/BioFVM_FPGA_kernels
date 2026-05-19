#include "broadcast_x3.hpp"
#include "defines.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Create test inputs.
    const uint32_t line_length = 10;
    const uint32_t num_lines_per_group = 17;
    const uint32_t N = num_lines_per_group * GROUP_SIZE * line_length;
    const uint32_t reduced_N = num_lines_per_group * GROUP_SIZE * (line_length-1);

    double density_pe[N];
    for (uint32_t i = 0; i < N; i++) {
        density_pe[i] = i * i;
    }

    hls::stream<double> from_pe_x3;
    for (uint32_t i = 0; i < N; i++) {
        from_pe_x3.write(density_pe[i]);
    }

    // Create correct (golden) outputs.
    double golden_loopback[reduced_N];
    int addr = 0;
    for (uint32_t i = 0; i < num_lines_per_group; i++) {
        for (uint32_t j = 0; j < line_length-1; j++) {
            for (uint32_t k = 0; k < GROUP_SIZE; k++) {
                golden_loopback[addr] = density_pe[i*GROUP_SIZE*line_length + j*GROUP_SIZE + k];
                addr++;
            }
        }
    }
    double golden_intermediate[N];
    memcpy(golden_intermediate, density_pe, sizeof(double)*N);

    hls::stream<double> density_loopback;
    hls::stream<double> density_intermediate;

    // Run the kernel as a C++ function.
    broadcast_x3(from_pe_x3, density_loopback, density_intermediate, line_length, num_lines_per_group);

    // Read the outputs from the stream.
    double loopback[reduced_N];
    for (uint32_t words_read = 0; words_read < reduced_N; words_read++) {
        loopback[words_read] = density_loopback.read();
    }
    double intermediate[N];
    for (uint32_t words_read = 0; words_read < N; words_read++) {
        intermediate[words_read] = density_intermediate.read();
    }

    // Check the results.
    bool pass = true;
    for (uint32_t i = 0; i < reduced_N; i++) {
        if (loopback[i] != golden_loopback[i]) {
            std::cout << "loopback: Mismatch at index " << i << ": expected " << golden_loopback[i] << ", got " << loopback[i] << std::endl;
            pass = false;
        }
    }
    for (uint32_t i = 0; i < N; i++) {
        if (intermediate[i] != golden_intermediate[i]) {
            std::cout << "intermediate: Mismatch at index " << i << ": expected " << golden_intermediate[i] << ", got " << intermediate[i] << std::endl;
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