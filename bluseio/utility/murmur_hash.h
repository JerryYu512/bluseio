#ifndef BLUSEIO_MURMUR_HASH_H_
#define BLUSEIO_MURMUR_HASH_H_

#include <stdint.h>
#include <stddef.h>

namespace blsueio {
namespace utility {
uint32_t murmur_hash32(const void* key, size_t len, uint32_t seed);
uint64_t murmur_hash64(const void* key, size_t len, uint64_t seed);
}
}

#endif // BLUSEIO_MURMUR_HASH_H_