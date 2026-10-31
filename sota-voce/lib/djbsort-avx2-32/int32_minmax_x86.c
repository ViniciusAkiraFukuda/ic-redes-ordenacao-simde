#define int32_MINMAX(a,b) \
do { \
  int32_t ab = b ^ a; \
  int32_t c = b - a; \
  c ^= ab & (c ^ b); \
  c >>= 31; \
  c &= ab; \
  a ^= c; \
  b ^= c; \
} while(0)