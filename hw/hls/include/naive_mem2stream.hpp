#ifndef NAIVE_MEM2STREAM_HPP
#define NAIVE_MEM2STREAM_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Dump memory contents to stream for testing
void naive_mem2stream(
  ap_uint<REAL_WIDTH> *mem,
  hls::stream<ap_uint<REAL_WIDTH>> &stream,
  int N
);
}

#endif // NAIVE_MEM2STREAM_HPP