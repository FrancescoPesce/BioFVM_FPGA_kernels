#ifndef FORWARD_PE_HPP
#define FORWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Compute (x1 * c1 + x2) * c2
void forward_pe(
  hls::stream<ap_uint<REAL_WIDTH>> &x1,
  hls::stream<ap_uint<REAL_WIDTH>> &x2,
  hls::stream<ap_uint<REAL_WIDTH>> &c1,
  hls::stream<ap_uint<REAL_WIDTH>> &c2,
  hls::stream<ap_uint<REAL_WIDTH>> &x3,
  int N
);
}

#endif // FORWARD_PE_HPP