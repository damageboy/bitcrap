#include "bitgoo.h"

int32_t CountBits::NoPopcnt(uint64_t *bits, int32_t bitLength, int64_t offset)
{
  if (is_bit_set(bits, offset))
    return -1;

  auto index = 0;
  while (offset >= 63) {
    IACA_START
    index += BitCount64(*(bits++));
    offset -= 64;
  }
  IACA_END

  return offset >= 0 ?
         index + BitCount64(*bits & ((1ULL << (offset + 1)) - 1)) :
         index;
}