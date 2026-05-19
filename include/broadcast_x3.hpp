#ifndef BROADCAST_X3_HPP
#define BROADCAST_X3_HPP

#include <hls_stream.h>
#include <stdint.h>

extern "C" {
void broadcast_x3(
  hls::stream<double> &from_pe_x3,
  hls::stream<double> &density_loopback,
  hls::stream<double> &density_intermediate,
  int line_length,
  int num_lines_per_group
);
}

#endif // BROADCAST_X3_HPP