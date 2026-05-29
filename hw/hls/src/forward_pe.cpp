#include "forward_pe.hpp"

extern "C" {

void forward_pe(
  hls::stream<ap_uint<REAL_WIDTH>> &x1,
  hls::stream<ap_uint<REAL_WIDTH>> &x2,
  hls::stream<ap_uint<REAL_WIDTH>> &c1,
  hls::stream<ap_uint<REAL_WIDTH>> &c2,
  hls::stream<ap_uint<REAL_WIDTH>> &x3,
  int N
) {
#pragma HLS INTERFACE axis port=x1
#pragma HLS INTERFACE axis port=x2
#pragma HLS INTERFACE axis port=c1
#pragma HLS INTERFACE axis port=c2
#pragma HLS INTERFACE axis port=x3
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  unsigned_to_real conv = {0};

  bool vx1 = false, vx2 = false, vc1 = false, vc2 = false;
  ap_uint<REAL_WIDTH> bx1, bx2, bc1, bc2;

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

      conv.u = bx1; real_t x1_val = conv.r;
      conv.u = bx2; real_t x2_val = conv.r;
      conv.u = bc1; real_t c1_val = conv.r;
      conv.u = bc2; real_t c2_val = conv.r;

      real_t temp1 = x1_val * c1_val;
      real_t temp2 = temp1 + x2_val;
      real_t temp3 = temp2 * c2_val;

      conv.r = temp3;
      x3.write(conv.u);

      // Consume tuple
      vx1 = vx2 = vc1 = vc2 = false;
      i++;
    }
  }
}

}