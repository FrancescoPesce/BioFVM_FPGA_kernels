#include "naive_stream2mem.hpp"

extern "C" {

void naive_stream2mem(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int N
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  for (int i=0; i < N; ++i) {
    mem[i] = stream.read();
  }
}

}