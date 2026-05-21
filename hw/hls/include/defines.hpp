#ifndef DEFINES_HPP
#define DEFINES_HPP

#define LOG_GROUP_SIZE 5
#define GROUP_SIZE (1 << LOG_GROUP_SIZE)
#define GROUP_OFFSET_MASK (GROUP_SIZE-1)

#define LOG_MAX_DIM 11
#define MAX_DIM (1 << LOG_MAX_DIM)
#define MAX_S 8

union u64_to_double {
  ap_uint<64> u;
  double d;
};

#endif // DEFINES_HPP