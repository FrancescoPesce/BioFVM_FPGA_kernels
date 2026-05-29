#ifndef PE_LOOPBACK_HPP
#define PE_LOOPBACK_HPP

#include <hls_stream.h>
#include <stdint.h>
#include <ap_int.h>
#include "defines.hpp"

extern "C" {
// Broadcast the output of forward/backward_pe (from_pe_x3) to 
// - the rest of the computation (density_output): full stream 
// - forward/backward_pe (density_loopback): add GROUP_SIZE 0s and remove last GROUP_SIZE values in each line
void pe_loopback(
  hls::stream<ap_uint<REAL_WIDTH>> &from_pe_x3,
  hls::stream<ap_uint<REAL_WIDTH>> &density_loopback,
  hls::stream<ap_uint<REAL_WIDTH>> &density_output,
  int line_length,
  int num_lines_per_group
);
}

#endif // PE_LOOPBACK_HPP