#include "forward_pe.hpp"
#include "defines.hpp"

extern "C" {

void forward_pe(
  hls::stream<ap_uint<64>> &x1,
  hls::stream<ap_uint<64>> &x2,
  hls::stream<ap_uint<64>> &c1,
  hls::stream<ap_uint<64>> &c2,
  hls::stream<ap_uint<64>> &x3,
  int len
) {
#pragma HLS INTERFACE axis port=x1
#pragma HLS INTERFACE axis port=x2
#pragma HLS INTERFACE axis port=c1
#pragma HLS INTERFACE axis port=c2
#pragma HLS INTERFACE axis port=x3
#pragma HLS INTERFACE s_axilite port=len bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control 

  for (int i=0; i < len; ++i) {
#pragma HLS PIPELINE II=1
    u64_to_double conv = {0};

    conv.u = x1.read();
    double x1_val = conv.d;

    conv.u = c1.read();
    double c1_val = conv.d;

    conv.u = x2.read();
    double x2_val = conv.d;

    conv.u = c2.read();
    double c2_val = conv.d;

    double temp1 = x1_val * c1_val;
    double temp2 = temp1 + x2_val;
    double temp3 = temp2 * c2_val;

    conv.d = temp3;
    x3.write(conv.u);
  }
}

}