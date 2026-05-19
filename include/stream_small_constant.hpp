#ifndef STREAM_SMALL_CONSTANT_HPP
#define STREAM_SMALL_CONSTANT_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <string.h>

extern "C" {
void stream_small_constant(
  double *mem,
  hls::stream<double> &stream,
  int NV,
  int NS
);
}

#endif // STREAM_SMALL_CONSTANT_HPP