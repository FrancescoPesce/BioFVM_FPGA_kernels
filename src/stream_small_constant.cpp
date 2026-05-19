#include "stream_small_constant.hpp"
#include "defines.hpp"

extern "C" {

void stream_small_constant(
  double *mem,
  hls::stream<double> &stream,
  int NV,
  int NS
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream 
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=NV bundle=control
#pragma HLS INTERFACE s_axilite port=NS bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  double local_constant[MAX_S];
#pragma HLS BIND_STORAGE variable=local_constant type=ram_1p impl=lutram

  memcpy(local_constant, mem, sizeof(double)*NS);

  int base_addr = 0;
  for (int i=0; i < NS; ++i) {
    for (int j=0; j < NV; ++j) {
#pragma HLS PIPELINE II=1
      stream.write(local_constant[i]);
    }
  }
}

}