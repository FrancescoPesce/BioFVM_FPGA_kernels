#ifndef STREAM_LARGE_CONSTANT_HPP
#define STREAM_LARGE_CONSTANT_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>

extern "C" {
void stream_large_constant(
  double *mem,
  hls::stream<double> &stream,
  int line_length,
  int num_lines_per_S,
  int NS
);
}

#endif // STREAM_LARGE_CONSTANT_HPP