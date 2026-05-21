#ifndef STREAM_C_HPP
#define STREAM_C_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>

extern "C" {
// Save NS * line_length c values to cache,
// stream each line num_lines_per_S_group * GROUP_SIZE times,
// then change to the next S, for NS times
void stream_c(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int line_length,
  int num_lines_per_S_group,
  int NS
);
}

#endif // STREAM_C_HPP