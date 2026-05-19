#include "feed_forward_pe.hpp"
#include "defines.hpp"

extern "C" {

void feed_forward_pe(
  hls::stream<double> &density_from_ddr, // round-robin over GROUP_SIZE lines
  hls::stream<double> &density_from_pe, // loopback from forward_pe, same order as ddr stream
  hls::stream<double> &to_pe_x1,
  hls::stream<double> &to_pe_x2,
  int line_length,
  int num_lines_per_group
) {
#pragma HLS INTERFACE axis port=density_from_ddr
#pragma HLS INTERFACE axis port=density_from_pe
#pragma HLS INTERFACE axis port=to_pe_x1
#pragma HLS INTERFACE axis port=to_pe_x2
#pragma HLS INTERFACE s_axilite port=line_length bundle=control
#pragma HLS INTERFACE s_axilite port=num_lines_per_group bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  for (int i=0; i < num_lines_per_group; ++i) {
    for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
      to_pe_x1.write(0.0);
      to_pe_x2.write(density_from_ddr.read());
    }

    for (int j=1; j < line_length; ++j) {
      for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
        to_pe_x1.write(density_from_pe.read());
        to_pe_x2.write(density_from_ddr.read());
      }
    }
  }
}

}