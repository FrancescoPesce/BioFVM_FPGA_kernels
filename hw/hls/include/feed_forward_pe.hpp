#ifndef FEED_FORWARD_PE_HPP
#define FEED_FORWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
void feed_forward_pe(
  hls::stream<ap_uint<64>> &density_from_ddr,
  hls::stream<ap_uint<64>> &density_from_pe,
  hls::stream<ap_uint<64>> &to_pe_x1,
  hls::stream<ap_uint<64>> &to_pe_x2,
  int line_length,
  int num_lines_per_group
);
}

#endif // FEED_FORWARD_PE_HPP