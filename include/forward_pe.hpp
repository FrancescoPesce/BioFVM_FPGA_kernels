#ifndef FORWARD_PE_HPP
#define FORWARD_PE_HPP

#include <hls_stream.h>
#include <stdint.h>

extern "C" {
void forward_pe(
  hls::stream<double> &x1,
  hls::stream<double> &x2,
  hls::stream<double> &c1,
  hls::stream<double> &c2,
  hls::stream<double> &x3,
  int len
);
}

#endif // FORWARD_PE_HPP