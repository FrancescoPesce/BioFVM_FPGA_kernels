#ifndef FORWARD_PE_HPP
#define FORWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
// Compute (x1 * c1 + x2) * c2
void forward_pe(
  hls::stream<ap_uint<64>> &x1,
  hls::stream<ap_uint<64>> &x2,
  hls::stream<ap_uint<64>> &c1,
  hls::stream<ap_uint<64>> &c2,
  hls::stream<ap_uint<64>> &x3,
  int N
);
}

#endif // FORWARD_PE_HPP