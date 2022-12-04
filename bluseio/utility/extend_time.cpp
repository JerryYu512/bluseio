#include "extend_time.h"
#include <stdio.h>

namespace bluseio {

char* datetime_fmt_fs(datetime_t* dt, char* buf) {
    sprintf(buf, DATETIME_FMT_FS,
        dt->year, dt->month, dt->day,
        dt->hour, dt->min, dt->sec);
    return buf;
}
	
} // namespace bluseio
