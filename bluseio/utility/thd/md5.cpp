/**
 * MIT License
 * 
 * Copyright © 2022 <Jerry.Yu>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS";, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * @file md5.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-21
 * 
 * @copyright MIT License
 * 
 */
#include "md5.h"
#include <string.h>

namespace bluseio {
namespace utility {

static const uint8_t *md5_body(md5_t *ctx, const uint8_t *data, size_t size);

void md5_init(md5_t *ctx) {
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->bytes = 0;
}

void md5_update(md5_t *ctx, const void *data, size_t size) {
    size_t used, free;

    used = (size_t)(ctx->bytes & 0x3f);
    ctx->bytes += size;

    if (used) {
        free = 64 - used;

        if (size < free) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, free);
        data = (uint8_t *)data + free;
        size -= free;
        (void)md5_body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = md5_body(ctx, (const uint8_t *)data, size & ~(size_t)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

void md5_final(uint8_t result[16], md5_t *ctx) {
    size_t used, free;

    used = (size_t)(ctx->bytes & 0x3f);

    ctx->buffer[used++] = 0x80;

    free = 64 - used;

    if (free < 8) {
        memset(&ctx->buffer[used], 0, free);
        (void)md5_body(ctx, ctx->buffer, 64);
        used = 0;
        free = 64;
    }

    memset(&ctx->buffer[used], 0, free - 8);

    ctx->bytes <<= 3;
    ctx->buffer[56] = (uint8_t)ctx->bytes;
    ctx->buffer[57] = (uint8_t)(ctx->bytes >> 8);
    ctx->buffer[58] = (uint8_t)(ctx->bytes >> 16);
    ctx->buffer[59] = (uint8_t)(ctx->bytes >> 24);
    ctx->buffer[60] = (uint8_t)(ctx->bytes >> 32);
    ctx->buffer[61] = (uint8_t)(ctx->bytes >> 40);
    ctx->buffer[62] = (uint8_t)(ctx->bytes >> 48);
    ctx->buffer[63] = (uint8_t)(ctx->bytes >> 56);

    (void)md5_body(ctx, ctx->buffer, 64);

    result[0] = (uint8_t)ctx->a;
    result[1] = (uint8_t)(ctx->a >> 8);
    result[2] = (uint8_t)(ctx->a >> 16);
    result[3] = (uint8_t)(ctx->a >> 24);
    result[4] = (uint8_t)ctx->b;
    result[5] = (uint8_t)(ctx->b >> 8);
    result[6] = (uint8_t)(ctx->b >> 16);
    result[7] = (uint8_t)(ctx->b >> 24);
    result[8] = (uint8_t)ctx->c;
    result[9] = (uint8_t)(ctx->c >> 8);
    result[10] = (uint8_t)(ctx->c >> 16);
    result[11] = (uint8_t)(ctx->c >> 24);
    result[12] = (uint8_t)ctx->d;
    result[13] = (uint8_t)(ctx->d >> 8);
    result[14] = (uint8_t)(ctx->d >> 16);
    result[15] = (uint8_t)(ctx->d >> 24);

    memset(ctx, 0, sizeof(*ctx));
}

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#define STEP(f, a, b, c, d, x, t, s)                         \
    (a) += f((b), (c), (d)) + (x) + (t);                     \
    (a) = (((a) << (s)) | (((a)&0xffffffff) >> (32 - (s)))); \
    (a) += (b)

/*
 * SET() reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * does not work.
 */

#define SET(n) (*(uint32_t *)&p[n * 4])
#define GET(n) (*(uint32_t *)&p[n * 4])

/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

static const uint8_t *md5_body(md5_t *ctx, const uint8_t *data, size_t size) {
    uint32_t a, b, c, d;
    uint32_t saved_a, saved_b, saved_c, saved_d;
    const uint8_t *p;

    p = data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7);
        STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12);
        STEP(F, c, d, a, b, SET(2), 0x242070db, 17);
        STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22);
        STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7);
        STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12);
        STEP(F, c, d, a, b, SET(6), 0xa8304613, 17);
        STEP(F, b, c, d, a, SET(7), 0xfd469501, 22);
        STEP(F, a, b, c, d, SET(8), 0x698098d8, 7);
        STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12);
        STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17);
        STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22);
        STEP(F, a, b, c, d, SET(12), 0x6b901122, 7);
        STEP(F, d, a, b, c, SET(13), 0xfd987193, 12);
        STEP(F, c, d, a, b, SET(14), 0xa679438e, 17);
        STEP(F, b, c, d, a, SET(15), 0x49b40821, 22);

        /* Round 2 */

        STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5);
        STEP(G, d, a, b, c, GET(6), 0xc040b340, 9);
        STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20);
        STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5);
        STEP(G, d, a, b, c, GET(10), 0x02441453, 9);
        STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20);
        STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5);
        STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14);
        STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20);
        STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9);
        STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14);
        STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4);
        STEP(H, d, a, b, c, GET(8), 0x8771f681, 11);
        STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4);
        STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11);
        STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16);
        STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11);
        STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16);
        STEP(H, b, c, d, a, GET(6), 0x04881d05, 23);
        STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4);
        STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23);

        /* Round 4 */

        STEP(I, a, b, c, d, GET(0), 0xf4292244, 6);
        STEP(I, d, a, b, c, GET(7), 0x432aff97, 10);
        STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21);
        STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10);
        STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21);
        STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6);
        STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        STEP(I, c, d, a, b, GET(6), 0xa3014314, 15);
        STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6);
        STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15);
        STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return p;
}

}
}