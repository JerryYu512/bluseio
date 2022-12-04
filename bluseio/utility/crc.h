#ifndef BLUSEIO_CRC_H_
#define BLUSEIO_CRC_H_

#include <stdint.h>
#include <stddef.h>

#include <string.h>
#include <string>

namespace bluseio {

static inline uint32_t crc_simple(uint8_t *data, size_t len)
{
    uint32_t  sum;

    for (sum = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        sum = sum >> 1 | sum << 31;

        sum += *data++;
    }

    return sum;
}

uint16_t crc16(const void* s, size_t n, uint16_t crc);

static inline uint16_t crc16(const void* s, size_t n) {
    return bluseio::crc16(s, n, 0);
}

static inline uint16_t crc16(const char* s) {
    return crc16(s, ::strlen(s));
}

template<typename S>
inline uint16_t crc16(const S& s) {
    return crc16(s.data(), s.size());
}

#define bluseio_crc32_init(crc) crc = 0xffffffff
#define bluseio_crc32_final(crc) crc ^= 0xffffffff
uint32_t crc32(uint32_t crc, const uint8_t *buffer, uint32_t size);

}

#endif // BLUSEIO_CRC_H_
