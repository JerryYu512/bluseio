#ifndef BLUSEIO_CRC_H_
#define BLUSEIO_CRC_H_

#include <stdint.h>
#include <stddef.h>

#include <string.h>
#include <string>

namespace bluseio {

uint16_t crc16(const void* s, size_t n, uint16_t crc);
uint32_t crc32(uint32_t crc, const uint8_t *buffer, uint32_t size);


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

}

#endif // BLUSEIO_CRC_H_
