#ifndef uint64_sort_H
#define uint64_sort_H

#include <stddef.h>
#include <stdint.h>

// Modified in NTRU-sampling to match PQC_NEON
#ifndef CRYPTO_NAMESPACE
#define CRYPTO_NAMESPACE(s) s
#endif

#define uint64_sort CRYPTO_NAMESPACE(uint64_sort)

void uint64_sort(uint64_t *x,long long n);

#endif
