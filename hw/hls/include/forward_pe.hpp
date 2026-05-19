#ifndef FORWARD_PE_HPP
#define FORWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {

union u64_to_double {
  ap_uint<64> u;
  double d;
};

void forward_pe(
  hls::stream<ap_uint<64>> &x1,
  hls::stream<ap_uint<64>> &x2,
  hls::stream<ap_uint<64>> &c1,
  hls::stream<ap_uint<64>> &c2,
  hls::stream<ap_uint<64>> &x3,
  int len
);
}

#endif // FORWARD_PE_HPP