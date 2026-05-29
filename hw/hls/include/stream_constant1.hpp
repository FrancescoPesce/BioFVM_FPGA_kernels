#ifndef STREAM_CONSTANT1_HPP
#define STREAM_CONSTANT1_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Save NS constant1 values to cache and stream each value NV times
void stream_constant1(
  ap_uint<REAL_WIDTH> *mem,
  hls::stream<ap_uint<REAL_WIDTH>> &stream,
  int NV,
  int NS
);
}

#endif // STREAM_CONSTANT1_HPP