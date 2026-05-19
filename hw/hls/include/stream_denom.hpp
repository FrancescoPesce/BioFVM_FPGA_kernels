#ifndef STREAM_DENOM_HPP
#define STREAM_DENOM_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>

extern "C" {
void stream_denom(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int line_length,
  int num_lines_per_S_group,
  int NS
);
}

#endif // STREAM_DENOM_HPP