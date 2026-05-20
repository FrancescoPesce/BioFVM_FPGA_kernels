#include "broadcast_x3.hpp"
#include "defines.hpp"

extern "C" {

void broadcast_x3(
  hls::stream<ap_uint<64>> &from_pe_x3,
  hls::stream<ap_uint<64>> &density_loopback,
  hls::stream<ap_uint<64>> &density_intermediate, // round-robin over GROUP_SIZE lines
  int line_length,
  int num_lines_per_group
) {
#pragma HLS INTERFACE axis port=from_pe_x3
#pragma HLS INTERFACE axis port=density_loopback
#pragma HLS INTERFACE axis port=density_intermediate
#pragma HLS INTERFACE s_axilite port=line_length bundle=control
#pragma HLS INTERFACE s_axilite port=num_lines_per_group bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  u64_to_double conv = {0};

  // Initialize loopback with one 0 per line
  for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
    conv.d = 0.0;
    density_loopback.write(conv.u);
  }

  // Main loop: copy x3 to both loopback and intermediate, 
  // write 0 into loopback for the final GROUP_SIZE entries in each line for the next iteration
  for (int i=0; i < num_lines_per_group - 1; ++i) {
    for (int j=0; j < line_length - 1; ++j) {
#pragma HLS PIPELINE II=GROUP_SIZE
      for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
        ap_uint<64> val = from_pe_x3.read();
        density_loopback.write(val);
        density_intermediate.write(val);
      }
    }

    for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
      conv.d = 0.0;
      density_loopback.write(conv.u);
      density_intermediate.write(from_pe_x3.read());
    }
  }

  // At the last iteration, do not write 0s into the loopback
  for (int j=0; j < line_length - 1; ++j) {
#pragma HLS PIPELINE II=GROUP_SIZE
    for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
      ap_uint<64> val = from_pe_x3.read();
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