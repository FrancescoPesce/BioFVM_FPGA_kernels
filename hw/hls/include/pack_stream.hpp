#ifndef PACK_STREAM_HPP
#define PACK_STREAM_HPP

#include <hls_stream.h>
#include <stdint.h>
#define AP_INT_MAX_W 4096
#include <ap_int.h>
#include "defines.hpp"

extern "C" {

// Pack GROUP_SIZE narrow values in a wide one
void pack_stream(
  hls::stream<ap_uint<64>>& in,
  hls::stream<ap_uint<64 * GROUP_SIZE>>& out,
  int N_per_group
);

}

#endif // PACK_STREAM_HPP