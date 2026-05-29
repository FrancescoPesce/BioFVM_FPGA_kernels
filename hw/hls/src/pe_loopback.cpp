#include "pe_loopback.hpp"

extern "C" {

void pe_loopback(
  hls::stream<ap_uint<REAL_WIDTH>> &from_pe_x3,
  hls::stream<ap_uint<REAL_WIDTH>> &density_loopback,
  hls::stream<ap_uint<REAL_WIDTH>> &density_output, // round-robin over GROUP_SIZE lines
  int line_length,
  int num_lines_per_group
) {
#pragma HLS INTERFACE axis port=from_pe_x3
#pragma HLS INTERFACE axis port=density_loopback
#pragma HLS INTERFACE axis port=density_output
#pragma HLS INTERFACE s_axilite port=line_length bundle=control
#pragma HLS INTERFACE s_axilite port=num_lines_per_group bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  uREAL_WIDTH_to_real_t conv = {0};

  // Initialize loopback with one 0 per line
  for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
    conv.d = 0.0;
    density_loopback.write(conv.u);
  }

  // Main loop: copy x3 to both loopback and intermediate, 
  // write 0 into loopback for the final GROUP_SIZE entries in each line for the next iteration
  // At the last iteration, do not write 0s into the loopback
  for (int i=0; i < num_lines_per_group; ++i) {
    for (int j=0; j < line_length; ++j) {
#pragma HLS PIPELINE II=GROUP_SIZE
      for (int k=0; k < GROUP_SIZE; ++k) {
#pragma HLS PIPELINE II=1
        ap_uint<REAL_WIDTH> val1 = from_pe_x3.read();
        density_output.write(val1);

        ap_uint<REAL_WIDTH> val2;
        if (j != (line_length-1)) {
          val2 = val1;
        } else {
          conv.d = 0.0;
          val2 = conv.u;
        }
        if ((i != (num_lines_per_group-1)) || (j != (line_length-1))) {
          density_loopback.write(val2);
        }
      }
    }
  }
}

}