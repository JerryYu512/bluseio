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
 * @file version.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-20
 * 
 * @copyright MIT License
 * 
 */
#include "version.h"
#include <stdio.h>

namespace bluseio {

const char* version(void) {
	static char __version[32] = "";
    char week[32] = "";
	char zone[16] = "";
	int year = 0;
	int mon = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;

    sscanf(BLUSEIO_COMPILE_DATETIME, "%s %d-%d-%d %d:%d:%d %s", week, &year, &mon, &day, &hour, &min, &sec, zone);
	snprintf(__version, sizeof(__version), "v%s %02d%02d%02d", BIO_VERSION_STRING, year % 100, mon, day);

	return __version;
}

const char* compile_time(void) {
	static char __time[32] = "";
    char week[32] = "";
	char zone[16] = "";
	int year = 0;
	int mon = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;

    sscanf(BLUSEIO_COMPILE_DATETIME, "%s %d-%d-%d %d:%d:%d %s", week, &year, &mon, &day, &hour, &min, &sec, zone);
	snprintf(__time, sizeof(__time), "%02d-%02d-%02d %02d:%02d:%02d", year % 100, mon, day, hour, min, sec);

	return __time;
}

}