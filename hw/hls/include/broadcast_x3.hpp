#ifndef BROADCAST_X3_HPP
#define BROADCAST_X3_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
// Broadcast the output of forward_pe (from_pe_x3) to 
// - the rest of the computation (density_intermediate): full stream 
// - feed_forward_pe (density_loopback): remove last GROUP_SIZE values of each line
void broadcast_x3(
  hls::stream<ap_uint<64>> &from_pe_x3,
  hls::stream<ap_uint<64>> &density_loopback,
  hls::stream<ap_uint<64>> &density_intermediate,
  int line_length,
  int num_lines_per_group
);
}

#endif // BROADCAST_X3_HPP