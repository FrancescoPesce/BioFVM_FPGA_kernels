#ifndef UNPACK_STREAM_HPP
#define UNPACK_STREAM_HPP

#include <hls_stream.h>
#include <stdint.h>
#define AP_INT_MAX_W 4096
#include <ap_int.h>
#include "defines.hpp"

extern "C" {

// Unpack one wide value into GROUP_SIZE narrow ones
void unpack_stream(
  hls::stream<ap_uint<64 * GROUP_SIZE>>& in,
  hls::stream<ap_uint<64>>& out,
  int N_per_group
);

}

#endif // UNPACK_STREAM_HPP