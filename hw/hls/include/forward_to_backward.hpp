#ifndef FORWARD_TO_BACKWARD_HPP
#define FORWARD_TO_BACKWARD_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
// Reverse the order of data within each line for each entry in a group,
// keeping groups of GROUP_SIZE data in the same order
void forward_to_backward(
  hls::stream<ap_uint<64>> &density_forward,
  hls::stream<ap_uint<64>> &density_backward,
  int line_length,
  int num_lines_per_group
);
}

#endif // FORWARD_TO_BACKWARD_HPP