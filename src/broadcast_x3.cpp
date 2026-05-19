#include "broadcast_x3.hpp"
#include "defines.hpp"

extern "C" {

void broadcast_x3(
  hls::stream<double> &from_pe_x3,
  hls::stream<double> &density_loopback,
  hls::stream<double> &density_intermediate, // round-robin over GROUP_SIZE lines
  int line_length,
  int num_lines_per_group
) {
  for (int i=0; i < num_lines_per_group; ++i) {
    for (int j=0; j < line_length - 1; ++j) {
      for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
        double val = from_pe_x3.read();
        density_loopback.write(val);
        density_intermediate.write(val);
      }
    }

    for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
      density_intermediate.write(from_pe_x3.read());
    }
  }
}

}