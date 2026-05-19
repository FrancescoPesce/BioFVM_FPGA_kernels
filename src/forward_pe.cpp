#include "forward_pe.hpp"
#include "defines.hpp"

extern "C" {

void forward_pe(
  hls::stream<double> &x1,
  hls::stream<double> &x2,
  hls::stream<double> &c1,
  hls::stream<double> &c2,
  hls::stream<double> &x3,
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
    double temp1 = x1.read() * c1.read();
    double temp2 = temp1 + x2.read();
    double temp3 = temp2 * c2.read();
    x3.write(temp3);
  }
}

}