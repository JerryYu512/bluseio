#ifndef BLUSEIO_RANDOM_H_
#define BLUSEIO_RANDOM_H_

#include <stdint.h>
#include <stddef.h>

namespace bluseio {
namespace utility {

typedef struct random_s {
	uint32_t seed;
} random_t;

static inline uint32_t random(random_t *seed) {
	if (seed->seed == 0 || seed->seed == 2147483647L) seed->seed = 1;

	const uint32_t M = 2147483647L;  // 2^31-1
	const uint64_t A = 16385;        // 2^14+1

	// Computing _seed * A % M.
	uint64_t p = seed->seed * A;
	seed->seed = (uint32_t)((p >> 31) + (p & M));
	if (seed->seed > M) seed->seed -= M;

	return seed->seed;
}

}
}
#endif // BLUSEIO_RANDOM_H_