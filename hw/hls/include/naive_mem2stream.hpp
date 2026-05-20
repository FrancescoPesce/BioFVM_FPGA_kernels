#ifndef NAIVE_MEM2STREAM_HPP
#define NAIVE_MEM2STREAM_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>

extern "C" {
// Dump memory contents to stream for testing
void naive_mem2stream(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int N
);
}

#endif // NAIVE_MEM2STREAM_HPP