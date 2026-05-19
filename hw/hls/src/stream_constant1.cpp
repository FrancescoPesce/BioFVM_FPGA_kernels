#include "stream_constant1.hpp"
#include "defines.hpp"

extern "C" {

void stream_constant1(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int NV,
  int NS
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream 
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=NV bundle=control
#pragma HLS INTERFACE s_axilite port=NS bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  ap_uint<64> local_constant[MAX_S];
#pragma HLS BIND_STORAGE variable=local_constant type=ram_1p impl=lutram

  memcpy(local_constant, mem, sizeof(ap_uint<64>)*NS);

  int base_addr = 0;
  for (int i=0; i < NS; ++i) {
    for (int j=0; j < NV; ++j) {
#pragma HLS PIPELINE II=1
      stream.write(local_constant[i]);
    }
  }
}

}