#ifndef BACKWARD_PE_HPP
#define BACKWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Compute x2 - x1 * c
void backward_pe(
  hls::stream<ap_uint<REAL_WIDTH>> &x1,
  hls::stream<ap_uint<REAL_WIDTH>> &x2,
  hls::stream<ap_uint<REAL_WIDTH>> &c,
  hls::stream<ap_uint<REAL_WIDTH>> &x3,
  int N
);
}

#endif // BACKWARD_PE_HPP