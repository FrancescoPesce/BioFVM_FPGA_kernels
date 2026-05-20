#include "forward_pe.hpp"
#include "defines.hpp"

extern "C" {

void forward_pe(
  hls::stream<ap_uint<64>> &x1,
  hls::stream<ap_uint<64>> &x2,
  hls::stream<ap_uint<64>> &c1,
  hls::stream<ap_uint<64>> &c2,
  hls::stream<ap_uint<64>> &x3,
  int N
) {
#pragma HLS INTERFACE axis port=x1
#pragma HLS INTERFACE axis port=x2
#pragma HLS INTERFACE axis port=c1
#pragma HLS INTERFACE axis port=c2
#pragma HLS INTERFACE axis port=x3
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  u64_to_double conv = {0};

  bool vx1 = false, vx2 = false, vc1 = false, vc2 = false;
  ap_uint<64> bx1, bx2, bc1, bc2;

  int i = 0;
  while (i < N) {
#pragma HLS PIPELINE II=1

    // Try to capture inputs without losing them
    if (!vx1 && x1.read_nb(bx1)) vx1 = true;
    if (!vx2 && x2.read_nb(bx2)) vx2 = true;
    if (!vc1 && c1.read_nb(bc1)) vc1 = true;
    if (!vc2 && c2.read_nb(bc2)) vc2 = true;

    // Only compute when full tuple is ready
    if (vx1 && vx2 && vc1 && vc2) {

      conv.u = bx1; double x1_val = conv.d;
      conv.u = bx2; double x2_val = conv.d;
      conv.u = bc1; double c1_val = conv.d;
      conv.u = bc2; double c2_val = conv.d;

      double temp1 = x1_val * c1_val;
      double temp2 = temp1 + x2_val;
      double temp3 = temp2 * c2_val;

      conv.d = temp3;
      x3.write(conv.u);

      // Consume tuple
      vx1 = vx2 = vc1 = vc2 = false;
      i++;
    }
  }
}

}