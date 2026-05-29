#include "stream_c.hpp"

extern "C" {

void stream_c(
  ap_uint<REAL_WIDTH> *mem,
  hls::stream<ap_uint<REAL_WIDTH>> &stream,
  int line_length,
  int num_lines_per_S_group,
  int NS
) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE axis port=stream 
#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=line_length bundle=control
#pragma HLS INTERFACE s_axilite port=num_lines_per_S_group bundle=control
#pragma HLS INTERFACE s_axilite port=NS bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  ap_uint<REAL_WIDTH> local_constant[MAX_S * MAX_DIM];
#pragma HLS BIND_STORAGE variable=local_constant type=ram_1p impl=bram

  memcpy(local_constant, mem, sizeof(ap_uint<REAL_WIDTH>)*NS*line_length);

  int base_addr = 0;
  for (int i=0; i < NS; ++i) {
    for (int j=0; j < num_lines_per_S_group; ++j) {
      for (int k=line_length-1; k >= 0; --k) {
        for (int l=0; l < GROUP_SIZE; ++l) {
#pragma HLS PIPELINE II=1
          stream.write(local_constant[base_addr + k]);
        }
      }
    }
    base_addr += line_length;
  }
}

}