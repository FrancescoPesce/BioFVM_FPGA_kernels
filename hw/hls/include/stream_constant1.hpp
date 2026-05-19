#ifndef STREAM_CONSTANT1_HPP
#define STREAM_CONSTANT1_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>

extern "C" {
void stream_constant1(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int NV,
  int NS
);
}

#endif // STREAM_CONSTANT1_HPP