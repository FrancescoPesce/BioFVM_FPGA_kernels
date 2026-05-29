#include "naive_stream2mem.hpp"

extern "C" {

void naive_stream2mem(
  ap_uint<REAL_WIDTH> *mem,
  hls::stream<ap_uint<REAL_WIDTH>> &stream,
  int N
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Note: obviously not feasible on hardware
  // This is a dummy kernel to sink the outputs from a stream
  // Even in simulation, MAX_S * MAX_DIM^3 results in an overflow
  ap_uint<REAL_WIDTH> local_mem[MAX_S * MAX_DIM];

  for (int i=0; i < N; ++i) {
#pragma HLS PIPELINE II=1
    local_mem[i] = stream.read();
  }

  memcpy(mem, local_mem, sizeof(ap_uint<REAL_WIDTH>)*N);
}

}