#ifndef BACKWARD_PE_HPP
#define BACKWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
// Compute x2 - x1 * c
void backward_pe(
  hls::stream<ap_uint<64>> &x1,
  hls::stream<ap_uint<64>> &x2,
  hls::stream<ap_uint<64>> &c,
  hls::stream<ap_uint<64>> &x3,
  int N
);
}

#endif // BACKWARD_PE_HPP