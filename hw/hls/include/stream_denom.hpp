#ifndef STREAM_DENOM_HPP
#define STREAM_DENOM_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>
#include <ap_int.h>

extern "C" {
// Save NS * line_length denom values to cache,
// stream each line num_lines_per_S_group * GROUP_SIZE times,
// then change to the next S, for NS times
void stream_denom(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int line_length,
  int num_lines_per_S_group,
  int NS
);
}

#endif // STREAM_DENOM_HPP