#pragma once

#define BITMAP_SET(p, n) ((p)[(n) >> 3] |= (1 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_CLR(p, n) ((p)[(n) >> 3] &= ~(1 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_GET(p, n) ((p)[(n) >> 3] & (1 << ((n) & (CHAR_BIT - 1))))

/// 数值对齐
#define NUM_ALIGN(d, a) (((d) + (a - 1)) & ~(a - 1))
/// 指针对齐
#define PTR_ALIGN(p, a) (uint8_t *)(((uintptr_t)(p) + ((uintptr_t)a - 1)) & ~((uintptr_t)a - 1))
