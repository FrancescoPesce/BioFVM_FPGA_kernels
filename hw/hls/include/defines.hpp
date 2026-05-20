#ifndef DEFINES_HPP
#define DEFINES_HPP

#define GROUP_SIZE 32

#define MAX_DIM 2048
#define MAX_S 8

union u64_to_double {
  ap_uint<64> u;
  double d;
};

#endif // DEFINES_HPP