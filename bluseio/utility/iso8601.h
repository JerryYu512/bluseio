#pragma once
#include <stdint.h>
#include <time.h>

namespace bluseio {
namespace utility {

typedef struct iso8601_s {
    int64_t sec;    /* Number of seconds since the epoch of 1970-01-01T00:00:00Z */
    int32_t nsec;   /* Nanoseconds [0, 999999999] */
    int16_t offset; /* Offset from UTC in minutes [-1439, 1439] */
} iso8601_t;

iso8601_t* iso8601_parse(iso8601_t* t, char* buf);
// precision > 0 && precision < 9
char* iso8601_format(iso8601_t* t, char* buf, int len, int precision);
time_t iso8601_mktime(iso8601_t* t, bool local);
	
}
} // namespace bluseio
