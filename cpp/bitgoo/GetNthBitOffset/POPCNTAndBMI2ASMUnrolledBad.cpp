#include <cstdio>
#include "bitgoo.h"

int32_t GetNthBitOffset::POPCNTAndBMI2ASMUnrolled2Bad(uint64_t *bits, int32_t bitLength, int64_t n)
{
  auto q_start = bits;
  // These are just fake variables used to
  uint64_t c0,c1;//,c2,c3;

  for (; n >= 256; bits += 4) {
    IACA_START
    __asm__(
      "popcnt %[q0], %[c0]\n\t"
      "sub %[c0], %[n]\n\t"
      "popcnt %[q1], %[c1]\n\t"
      "sub %[c1], %[n]\n\t"
      "popcnt %[q2], %[c1]\n\t"
      "sub %[c1], %[n]\n\t"
      "popcnt %[q3], %[c1]\n\t"
      "sub %[c1], %[n]\n\t"
    : [n] "+r" (n), [c0] "=r" (c0), [c1] "=r" (c1)//, [c2] "=r" (c2), [c3] "=r" (c3)
    : [q0] "m" (bits[0]), [q1] "m"  (bits[1]), [q2] "m"  (bits[2]), [q3] "m"  (bits[3]));
  }
  IACA_END

  // As long as we are still looking for more than 64 bits
  auto prevIndex = n;
  while (n > 0) {
    prevIndex = n;
    n -= _mm_popcnt_u64(*(bits++));
  }

  // We still need to look inside the last 64 bit word...
  if (prevIndex > 0) {
    // q is already pointing into the partial word
    // so we substract it when * 64
    auto fullWords = ((bits - q_start - 1) * 64);
    auto pos = _tzcnt_u64(_pdep_u64(1ULL << (prevIndex - 1), *(bits - 1)));
    return fullWords + pos;
  }

  return (bits - q_start) * 64 - 1;
}