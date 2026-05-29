#include "backward_pe.hpp"

extern "C" {

void backward_pe(
  hls::stream<ap_uint<REAL_WIDTH>> &x1,
  hls::stream<ap_uint<REAL_WIDTH>> &x2,
  hls::stream<ap_uint<REAL_WIDTH>> &c,
  hls::stream<ap_uint<REAL_WIDTH>> &x3,
  int N
) {
#pragma HLS INTERFACE axis port=x1
#pragma HLS INTERFACE axis port=x2
#pragma HLS INTERFACE axis port=c
#pragma HLS INTERFACE axis port=x3
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  unsigned_to_real conv = {0};

  bool vx1 = false, vx2 = false, vc = false;
  ap_uint<REAL_WIDTH> bx1, bx2, bc;

  int i = 0;
  while (i < N) {
#pragma HLS PIPELINE II=1

    // Try to capture inputs without losing them
    if (!vx1 && x1.read_nb(bx1)) vx1 = true;
    if (!vx2 && x2.read_nb(bx2)) vx2 = true;
    if (!vc && c.read_nb(bc)) vc = true;

    // Only compute when full tuple is ready
    if (vx1 && vx2 && vc) {

      conv.u = bx1; real_t x1_val = conv.r;
      conv.u = bx2; real_t x2_val = conv.r;
      conv.u = bc; real_t c_val = conv.r;

      real_t temp1 = x1_val * c_val;
      real_t temp2 = x2_val - temp1;

      conv.r = temp2;
      x3.write(conv.u);

      // Consume tuple
      vx1 = vx2 = vc = false;
      i++;
    }
  }
}

}