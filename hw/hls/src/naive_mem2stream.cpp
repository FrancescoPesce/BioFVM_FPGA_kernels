#include "naive_mem2stream.hpp"
#include "defines.hpp"

extern "C" {

void naive_mem2stream(
  ap_uint<64> *mem,
  hls::stream<ap_uint<64>> &stream,
  int N
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Note: obviously not feasible on hardware
  // This is a dummy kernel to dump the inputs into a stream
  // Even in simulation, MAX_S * MAX_DIM^3 results in an overflow
  ap_uint<64> local_mem[MAX_S * MAX_DIM];

  memcpy(local_mem, mem, sizeof(ap_uint<64>)*N);

  for (int i=0; i < N; ++i) {
#pragma HLS PIPELINE II=1
    stream.write(local_mem[i]);
  }
}

}