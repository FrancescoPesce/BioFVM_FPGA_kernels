#ifndef NAIVE_STREAM2MEM_HPP
#define NAIVE_STREAM2MEM_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Dump stream contents to memory for testing
void naive_stream2mem(
  ap_uint<REAL_WIDTH> *mem,
  hls::stream<ap_uint<REAL_WIDTH>> &stream,
  int N
);
}

#endif // NAIVE_STREAM2MEM_HPP