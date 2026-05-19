#ifndef BROADCAST_X3_HPP
#define BROADCAST_X3_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
void broadcast_x3(
  hls::stream<ap_uint<64>> &from_pe_x3,
  hls::stream<ap_uint<64>> &density_loopback,
  hls::stream<ap_uint<64>> &density_intermediate,
  int line_length,
  int num_lines_per_group
);
}

#endif // BROADCAST_X3_HPP