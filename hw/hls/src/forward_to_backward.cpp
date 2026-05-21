#include "forward_to_backward.hpp"
#include "defines.hpp"
#include "stdio.h"

extern "C" {

void forward_to_backward(
  hls::stream<ap_uint<64>> &density_forward,
  hls::stream<ap_uint<64>> &density_backward,
  int line_length,
  int num_lines_per_group
) {
#pragma HLS INTERFACE axis port=density_forward
#pragma HLS INTERFACE axis port=density_backward
#pragma HLS INTERFACE s_axilite port=line_length bundle=control
#pragma HLS INTERFACE s_axilite port=num_lines_per_group bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  ap_uint<64> buffer[GROUP_SIZE*(MAX_DIM+1)];
#pragma HLS BIND_STORAGE variable=buffer type=ram_t2p impl=uram

  // Initialize the buffer writing the first GROUP_SIZE lines
  int block_idx = 0;
  for (int j=0; j < line_length; ++j) {
#pragma HLS PIPELINE II=GROUP_SIZE
    for (int k=0; k < GROUP_SIZE; k++) {
      int addr = (block_idx << LOG_GROUP_SIZE) | k;
      buffer[addr] = density_forward.read();
    }
    block_idx++;
  }
  block_idx--;

  // Main loop: read old data from the buffer in reverse group order and write new data
  // At the last iteration, do not write new data
  bool iterating_up = false;
  for (int i=0; i < num_lines_per_group; ++i) {
    for (int j=0; j < line_length; ++j) {
#pragma HLS PIPELINE II=GROUP_SIZE
      for (int k=0; k < GROUP_SIZE; ++k) {
        int addr_backward = (block_idx << LOG_GROUP_SIZE) | k;
        density_backward.write(buffer[addr_backward]);

        if (i != (num_lines_per_group-1)) {
          int block_idx_forward = iterating_up ? (block_idx-1) : (block_idx+1);
          int addr_forward = (block_idx_forward << LOG_GROUP_SIZE) | k;
          
          buffer[addr_forward] = density_forward.read();
        }
      }

      if (iterating_up) {
        block_idx++;
        if (block_idx == line_length+1) {
          block_idx = line_length-1;
          iterating_up = false;
        }
      } else {
        block_idx--;
        if (block_idx == -1) {
          block_idx = 1;
          iterating_up = true;
        }
      }
    }
  }
}

}