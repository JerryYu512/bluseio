#pragma once

#include <hv/htime.h>

namespace bluseio {

#define DATETIME_FMT_FS     "%04d%02d%02d_%02d%02d%02d"

char* datetime_fmt_fs(datetime_t* dt, char* buf);
	
} // namespace bluseio
