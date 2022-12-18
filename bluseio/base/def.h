#pragma once
#include <limits.h>
#include <stdint.h>

/// 数值对齐
#define NUM_ALIGN(d, a) (((d) + (a - 1)) & ~(a - 1))
/// 指针对齐
#define PTR_ALIGN(p, a) (uint8_t *)(((uintptr_t)(p) + ((uintptr_t)a - 1)) & ~((uintptr_t)a - 1))

#define BITMAP_DEF(name, n, val) uint8_t name[NUM_ALIGN((n), CHAR_BIT)/CHAR_BIT] = {(val) ? UINT8_MAX : 0}
#define BITMAP_SET(p, n) ((p)[(n) >> 3] |= (0x01 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_CLR(p, n) ((p)[(n) >> 3] &= ~(0x01 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_GET(p, n) (((p)[(n) >> 3] & (0x01 << ((n) & (CHAR_BIT - 1)))) ? 1 : 0)
