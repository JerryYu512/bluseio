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
 * @file datetime.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-19
 * 
 * @copyright MIT License
 * 
 */
#include "datetime.h"
#include <assert.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>

namespace bluseio {
namespace utility  {

static const char* s_weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

static const char* s_months[] = {"January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"};

static const uint8_t s_days[] = \
//   1       3       5       7   8       10      12
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define EPOCH INT64_C(62135683200)  /* 1970-01-01T00:00:00 */
#define RDN_OFFSET INT64_C(62135683200)  /* 1970-01-01T00:00:00 */
#define MIN_SEC INT64_C(-62135596800) /* 0001-01-01T00:00:00 */
#define MAX_SEC INT64_C(253402300799) /* 9999-12-31T23:59:59 */

typedef struct iso8601_s {
    int64_t sec;    /* Number of seconds since the epoch of 1970-01-01T00:00:00Z */
    int32_t nsec;   /* Nanoseconds [0, 999999999] */
    int16_t offset; /* Offset from UTC in minutes [-1439, 1439] */
} iso8601_t;

static const uint32_t Pow10[10] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

static const uint16_t DayOffset[13] = {
    0, 306, 337, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275
};

static void _rdn_to_ymd(uint32_t rdn, uint16_t* yp, uint16_t* mp, uint16_t* dp) {
    uint32_t Z, H, A, B;
    uint16_t y, m, d;

    Z = rdn + 306;
    H = 100 * Z - 25;
    A = H / 3652425;
    B = A - (A >> 2);
    y = (100 * B + H) / 36525;
    d = B + Z - (1461 * y >> 2);
    m = (535 * d + 48950) >> 14;
    if (m > 12)
        y++, m -= 12;

    *yp = y;
    *mp = m;
    *dp = d - DayOffset[m];
}

static size_t _timestamp_format_internal(char* dst, size_t len, const iso8601_t* tsp,
                                        const int precision) {
    unsigned char *p;
    uint64_t sec;
    uint32_t rdn, v;
    uint16_t y, m, d;
    size_t dlen;

    dlen = sizeof("YYYY-MM-DDThh:mm:ssZ") - 1;
    if (tsp->offset)
        dlen += 5; /* hh:mm */

    if (precision)
        dlen += 1 + precision;

    if (dlen >= len)
        return 0;

    sec = tsp->sec + tsp->offset * 60 + EPOCH;
    rdn = sec / 86400;

    _rdn_to_ymd(rdn, &y, &m, &d);

   /*
    *           1
    * 0123456789012345678
    * YYYY-MM-DDThh:mm:ss
    */
    p = (unsigned char *)dst;
    v = sec % 86400;
    p[18] = '0' + (v % 10); v /= 10;
    p[17] = '0' + (v %  6); v /=  6;
    p[16] = ':';
    p[15] = '0' + (v % 10); v /= 10;
    p[14] = '0' + (v %  6); v /=  6;
    p[13] = ':';
    p[12] = '0' + (v % 10); v /= 10;
    p[11] = '0' + (v % 10);
    p[10] = 'T';
    p[ 9] = '0' + (d % 10); d /= 10;
    p[ 8] = '0' + (d % 10);
    p[ 7] = '-';
    p[ 6] = '0' + (m % 10); m /= 10;
    p[ 5] = '0' + (m % 10);
    p[ 4] = '-';
    p[ 3] = '0' + (y % 10); y /= 10;
    p[ 2] = '0' + (y % 10); y /= 10;
    p[ 1] = '0' + (y % 10); y /= 10;
    p[ 0] = '0' + (y % 10);
    p += 19;

    if (precision) {
        v = tsp->nsec / Pow10[9 - precision];
        switch (precision) {
            case 9: p[9] = '0' + (v % 10); v /= 10;
            case 8: p[8] = '0' + (v % 10); v /= 10;
            case 7: p[7] = '0' + (v % 10); v /= 10;
            case 6: p[6] = '0' + (v % 10); v /= 10;
            case 5: p[5] = '0' + (v % 10); v /= 10;
            case 4: p[4] = '0' + (v % 10); v /= 10;
            case 3: p[3] = '0' + (v % 10); v /= 10;
            case 2: p[2] = '0' + (v % 10); v /= 10;
            case 1: p[1] = '0' + (v % 10);
        }
        p[0] = '.';
        p += 1 + precision;
    }

    if (!tsp->offset) {

        *p++ = 'Z';
    } else {
        if (tsp->offset < 0)
            p[0] = '-', v = -tsp->offset;
        else
            p[0] = '+', v = tsp->offset;

        p[5] = '0' + (v % 10); v /= 10;
        p[4] = '0' + (v %  6); v /=  6;
        p[3] = ':';
        p[2] = '0' + (v % 10); v /= 10;
        p[1] = '0' + (v % 10);
        p += 6;
    }
    *p = 0;
    return dlen;
}

static int _parse_2d(const unsigned char* const p, size_t i, uint16_t* vp) {
    unsigned char d0, d1;
    if (((d0 = p[i + 0] - '0') > 9) ||
        ((d1 = p[i + 1] - '0') > 9))
        return 1;
    *vp = d0 * 10 + d1;
    return 0;
}

static int _parse_4d(const unsigned char* const p, size_t i, uint16_t* vp) {
    unsigned char d0, d1, d2, d3;
    if (((d0 = p[i + 0] - '0') > 9) ||
        ((d1 = p[i + 1] - '0') > 9) ||
        ((d2 = p[i + 2] - '0') > 9) ||
        ((d3 = p[i + 3] - '0') > 9))
        return 1;
    *vp = d0 * 1000 + d1 * 100 + d2 * 10 + d3;
    return 0;
}

iso8601_t iso8601_parse(const char* str) {
    iso8601_t t = {0, 0, 0};
    if (!str) {
        return t;
    }

    const unsigned char *cur, *end;
    unsigned char ch;
    uint16_t year, month, day, hour, min, sec;
    uint32_t rdn, sod, nsec;
    int16_t offset;
    size_t len = strlen(str);

    /*
     *           1
     * 01234567890123456789
     * 2013-12-31T23:59:59Z
     */
    cur = (const unsigned char*)str;
    if (len < 20 || cur[4] != '-' || cur[7] != '-' || cur[13] != ':' || cur[16] != ':') return t;

    ch = cur[10];
    if (!(ch == 'T' || ch == ' ' || ch == 't')) return t;

    if (_parse_4d(cur, 0, &year) || year < 1 || _parse_2d(cur, 5, &month) || month < 1 ||
        month > 12 || _parse_2d(cur, 8, &day) || day < 1 || day > 31 || _parse_2d(cur, 11, &hour) ||
        hour > 23 || _parse_2d(cur, 14, &min) || min > 59 || _parse_2d(cur, 17, &sec) || sec > 59)
        return t;

    if (day > 28 && day > DateTime::days_of_month(year, month)) return t;

    if (month < 3) year--;

    rdn = (1461 * year) / 4 - year / 100 + year / 400 + DayOffset[month] + day - 306;
    sod = hour * 3600 + min * 60 + sec;
    end = cur + len;
    cur = cur + 19;
    offset = nsec = 0;

    ch = *cur++;
    if (ch == '.') {
        const unsigned char* start;
        size_t ndigits;

        start = cur;
        for (; cur < end; cur++) {
            const unsigned char digit = *cur - '0';
            if (digit > 9) break;
            nsec = nsec * 10 + digit;
        }

        ndigits = cur - start;
        if (ndigits < 1 || ndigits > 9) return t;

        nsec *= Pow10[9 - ndigits];

        if (cur == end) return t;

        ch = *cur++;
    }

    if (!(ch == 'Z' || ch == 'z')) {
        /*
         *  01234
         * ±00:00
         */
        if (cur + 5 < end || !(ch == '+' || ch == '-') || cur[2] != ':') return t;

        if (_parse_2d(cur, 0, &hour) || hour > 23 || _parse_2d(cur, 3, &min) || min > 59) return t;

        offset = hour * 60 + min;
        if (ch == '-') offset *= -1;

        cur += 5;
    }

    if (cur != end) return t;

    t.sec = ((int64_t)rdn - 719163) * 86400 + sod - offset * 60;
    t.nsec = nsec;
    t.offset = offset;

    return t;
}

char* iso8601_format(iso8601_t* t, char* buf, int len, int precision) {
    if (!t || !buf) {
        return NULL;
    }

    if (precision < 0 || precision > 9) {
        return NULL;
    }

    if (_timestamp_format_internal(buf, len, t, precision)) {
        return buf;
    } else {
        return NULL;
    }
}

static void _rdn_to_struct_tm(uint32_t rdn, struct tm* tmp) {
    uint32_t Z, H, A, B;
    uint16_t C, y, m, d;

    Z = rdn + 306;
    H = 100 * Z - 25;
    A = H / 3652425;
    B = A - (A >> 2);
    y = (100 * B + H) / 36525;
    C = B + Z - (1461 * y >> 2);
    m = (535 * C + 48950) >> 14;
    if (m > 12)
        d = C - 306, y++, m -= 12;
    else
        d = C + 59 + ((y & 3) == 0 && (y % 100 != 0 || y % 400 == 0));

    tmp->tm_mday = C - DayOffset[m];    /* Day of month [1,31]           */
    tmp->tm_mon  = m - 1;               /* Month of year [0,11]          */
    tmp->tm_year = y - 1900;            /* Years since 1900              */
    tmp->tm_wday = rdn % 7;             /* Day of week [0,6] (Sunday =0) */
    tmp->tm_yday = d - 1;               /* Day of year [0,365]           */
}

time_t iso8601_mktime(iso8601_t* t, bool local) {
    if (!t) {
        return -1;
    }

    uint64_t sec;
    uint32_t rdn, sod;
    struct tm tmp;

    sec = t->sec + RDN_OFFSET;
    if (local)
        sec += t->offset * 60;
    rdn = sec / 86400;
    sod = sec % 86400;

    _rdn_to_struct_tm(rdn, &tmp);
    tmp.tm_sec  = sod % 60; sod /= 60;
    tmp.tm_min  = sod % 60; sod /= 60;
    tmp.tm_hour = sod;

    return mktime(&tmp);
}
static std::string get_timezone(void) {
	char buf[64] = "";
	FILE *fp = nullptr;
	fp = fopen("/etc/timezone", "r");
	if (fp) {
		fread(buf, sizeof(buf), 1, fp);
		fclose(fp);
	}

	return buf;
}

bool date_valid(int year, int month, int day)
{
	if (month > 12 || month < 1 || year < 1)
	{
		return false;
	}
	else
	{
		if (IS_LEAP_YEAR(year)) // 是闰年
		{
			if (month == 2)
			{
				if (day > 29 || day < 1)
					return false;
				else
					return true;
			}
		}
		if (month == 4 || month == 6 || month == 9 || month == 11)
		{
			if (day < 1 || day > 30)
				return false;
			else
				return true;
		}
		else if (month != 2)
		{
			if (day < 1 || day > 31)
				return false;
			else
				return true;
		}
		else
		{
			if (day < 1 || day > 28)
				return false;
			else
				return true;
		}
	}
	return true;
}

bool time_valid(time_t hour, time_t min, time_t sec, time_t ms) {
	if (hour > 23 || hour < 0) {
		return false;
	}

	if (min > 59 || min < 0) {
		return false;
	}

	if (sec > 60 || sec < 0) {
		return false;
	}

	if (ms > 999 || ms < 0) {
		return false;
	}

	return true;
}

static bool datetime_valid(const date_t& d) {
	return date_valid(d.year, d.month, d.day) && time_valid(d.hour, d.min, d.sec, d.ms);
}

DateTime::DateTime() {
    time_t utc = 0;
    time_t local = 0;
	struct tm tm;
	struct timeval tval;
	struct timezone tz;

	::memset(&tval, 0, sizeof(tval));
	::memset(&tm, 0, sizeof(tm));
	::memset(&tz, 0, sizeof(tz));

	gettimeofday(&tval, &tz);
	sec_ = tval.tv_sec;
	usec_ = tval.tv_usec;
	nsec_ = usec_ * 1000;

    gmtime_r(&sec_, &tm);
    utc = ::mktime(&tm);

	::memset(&tm, 0, sizeof(tm));
	localtime_r(&tval.tv_sec, &tm);
    local = ::mktime(&tm);

	min_offset_ = (local - utc) / 60;
	// dst_ = tz.tz_dsttime;
	zone_ = get_timezone();

	date_.year = tm.tm_year + 1900;
	date_.month = tm.tm_mon + 1;
	date_.day = tm.tm_mday;
	// date_.wday = tm.tm_wday;
	date_.hour = tm.tm_hour;
	date_.min = tm.tm_min;
	date_.sec = tm.tm_sec;
	date_.ms = usec_ / 1000;
}

DateTime::DateTime(time_t sec, time_t us, int16_t timezone_offset_min, const char* zone_name) {
	sec_ = sec;
	usec_ = us;
	nsec_ = usec_ * 1000;

    time_t utc = 0;
    time_t local = 0;
	struct tm tm;
	struct timeval tval;
	struct timezone tz;

	::memset(&tval, 0, sizeof(tval));
	::memset(&tm, 0, sizeof(tm));
	::memset(&tz, 0, sizeof(tz));

	if (INT16_MAX == timezone_offset_min) {
        gmtime_r(&sec_, &tm);
        utc = ::mktime(&tm);
		// gettimeofday(&tval, &tz);

        ::memset(&tm, 0, sizeof(tm));
        localtime_r(&sec, &tm);
        local = ::mktime(&tm);

		min_offset_ = (local - utc) / 3600;
		// dst_ = tz.tz_dsttime;
	} else {
		min_offset_ = timezone_offset_min;
		dst_ = 0;
        ::memset(&tm, 0, sizeof(tm));
        localtime_r(&sec, &tm);
	}
	if (zone_name) {
		zone_ = zone_name;
	} else {
		zone_ = get_timezone();
	}

	date_.year = tm.tm_year + 1900;
	date_.month = tm.tm_mon + 1;
	date_.day = tm.tm_mday;
	// date_.wday = tm.tm_wday;
	date_.hour = tm.tm_hour;
	date_.min = tm.tm_min;
	date_.sec = tm.tm_sec;
	date_.ms = usec_ / 1000;
}

DateTime::DateTime(const date_t& date, int16_t timezone_offset_min, const char* zone_name) {
    time_t utc = 0;
    time_t local = 0;
	struct tm tm;
	struct timeval tval;
	struct timezone tz;

	::memset(&tval, 0, sizeof(tval));
	::memset(&tz, 0, sizeof(tz));
	::memset(&tm, 0, sizeof(tm));

	date_ = date;
	tm.tm_year = date_.year - 1900;
	tm.tm_mon = date_.month - 1;
	tm.tm_mday = date_.day;
	// tm.tm_wday = date_.wday;
	tm.tm_hour = date_.hour;
	tm.tm_min = date_.min;
	tm.tm_sec = date_.sec;

	sec_ = ::mktime(&tm);
	usec_ = date_.ms * 1000;
	nsec_ = usec_ * 1000;

	if (INT16_MAX == timezone_offset_min) {
	    ::memset(&tm, 0, sizeof(tm));
        gmtime_r(&sec_, &tm);
        utc = ::mktime(&tm);
		// gettimeofday(&tval, &tz);

        ::memset(&tm, 0, sizeof(tm));
        localtime_r(&tval.tv_sec, &tm);
        local = ::mktime(&tm);

		min_offset_ = (local - utc) / 3600;
		// dst_ = tz.tz_dsttime;
	} else {
		min_offset_ = timezone_offset_min;
		dst_ = 0;
	}
	if (zone_name) {
		zone_ = zone_name;
	} else {
		zone_ = get_timezone();
	}
}

DateTime::DateTime(const struct tm& tm, int16_t timezone_offset_min, const char* zone_name) {
    time_t utc = 0;
    time_t local = 0;
	struct timeval tval;
	struct timezone tz;
    struct tm tm2;

	::memset(&tval, 0, sizeof(tval));
	::memset(&tz, 0, sizeof(tz));

	date_.year = tm.tm_year + 1900;
	date_.month = tm.tm_mon + 1;
	date_.day = tm.tm_mday;
	// date_.wday = tm.tm_wday;
	date_.hour = tm.tm_hour;
	date_.min = tm.tm_min;
	date_.sec = tm.tm_sec;
	date_.ms = usec_ / 1000;

	sec_ = ::mktime((struct tm*)&tm);
	usec_ = date_.ms * 1000;
	nsec_ = usec_ * 1000;

	if (INT16_MAX == timezone_offset_min) {
	    ::memset(&tm2, 0, sizeof(tm2));
        gmtime_r(&sec_, &tm2);
        utc = ::mktime(&tm2);
		// gettimeofday(&tval, &tz);

        ::memset(&tm2, 0, sizeof(tm2));
        localtime_r(&tval.tv_sec, &tm2);
        local = ::mktime(&tm2);

		min_offset_ = (local - utc) / 3600;
		// dst_ = tz.tz_dsttime;
	} else {
		min_offset_ = timezone_offset_min;
		dst_ = 0;
	}
	if (zone_name) {
		zone_ = zone_name;
	} else {
		zone_ = get_timezone();
	}
}

bool DateTime::operator==(const DateTime& d) const {
	return sec_ == d.sec_ && usec_ == d.usec_;
}

bool DateTime::valid() const {
	return datetime_valid(date_) && sec_ >= 0 && usec_ >= 0;
}

std::string DateTime::duration_str() {
	char buf[16] = "";
	snprintf(buf, sizeof(buf), BIO_TIME_FMT, date_.year, date_.month, date_.day);
	return buf;
}

std::string DateTime::datetime_str() {
	char buf[32] = "";
	snprintf(buf, sizeof(buf), BIO_DATETIME_FMT, date_.year, date_.month, date_.day, date_.hour, date_.min, date_.sec);
	return buf;
}

__attribute_used__ static std::string iso8601_str(iso8601_t& t, int precision = 0) {
    char s[128] = "";
    return iso8601_format(&t, s, sizeof(s), precision);
}

std::string DateTime::iso8601_str(int precision) {
	iso8601_t iso = {sec_, (int32_t)nsec_, min_offset_};
    char s[128] = "";
    return iso8601_format(&iso, s, sizeof(s), precision);
}

std::string DateTime::gmtime_str() {
	char buf[32] = "";
	struct tm tm;

	::memset(&tm, 0, sizeof(tm));
	localtime_r(&sec_, &tm);

    sprintf(buf, BIO_GMTIME_FMT,
        s_weekdays[tm.tm_wday],
        tm.tm_mday, s_months[tm.tm_mon], tm.tm_year + 1900,
        tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

std::string DateTime::file_str() {
	char buf[32] = "";
	snprintf(buf, sizeof(buf), BIO_DATETIME_FMT_FILE, date_.year, date_.month, date_.day, date_.hour, date_.min, date_.sec);
	return buf;
}

std::string DateTime::logtime_str() {
	char buf[32] = "";
	snprintf(buf, sizeof(buf), BIO_DATETIME_LOG_FMT, date_.year, date_.month, date_.day, date_.hour, date_.min, date_.sec, date_.ms);
	return buf;
}

std::string DateTime::date_str() {
	// TODO:
	return "";
}

time_t DateTime::seconds() {
	return sec_;
}

date_t DateTime::date(void) {
	return date_;
}

// static function
clock_t DateTime::clock() {
	return ::clock();
}

uint32_t DateTime::tick_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

uint64_t DateTime::thrtime_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*(unsigned long long)1000000 + ts.tv_nsec / 1000;
}

uint64_t DateTime::now_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*(unsigned long long)1000000 + tv.tv_usec;
}

uint64_t DateTime::now_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

time_t DateTime::now_sec(void) {
	return ::time(nullptr);
}

bool DateTime::valid(const date_t& date) {
	return DateTime(date).valid();
}

bool DateTime::valid(const struct tm& tm) {
	return DateTime(tm).valid();
}

std::string DateTime::utc_str(time_t sec, time_t us, int16_t timezone_offset_min) {
	return DateTime(sec, us, timezone_offset_min).iso8601_str();
}

std::string DateTime::local_str(time_t sec) {
	return DateTime(sec).datetime_str();
}

DateTime DateTime::iso8601_parse(const char* str) {
	iso8601_t iso = utility::iso8601_parse(str);
	return DateTime(iso.sec, iso.nsec * 1000, iso.offset);
}

int DateTime::days_of_month(int month, int year) {
    if (month < 1 || month > 12) {
        return 0;
    }
    int days = s_days[month-1];
    return (month == 2 && IS_LEAP_YEAR(year)) ? ++days : days;
}

int DateTime::month_atoi(const char* month) {
    for (size_t i = 0; i < 12; ++i) {
        if (strncmp(month, s_months[i], strlen(month)) == 0)
            return i+1;
    }
    return 0;
}

const char* DateTime::month_itoa(int month) {
    assert(month >= 1 && month <= 12);
    return s_months[month-1];
}

int DateTime::weekday_atoi(const char* weekday) {
    for (size_t i = 0; i < 7; ++i) {
        if (strncmp(weekday, s_weekdays[i], strlen(weekday)) == 0)
            return i;
    }
    return 0;
}

const char* DateTime::weekday_itoa(int weekday) {
    assert(weekday >= 0 && weekday <= 7);
    if (weekday == 7) weekday = 0;
    return s_weekdays[weekday];
}

void DateTime::sleep_us(uint64_t us) {
	std::this_thread::sleep_for(std::chrono::microseconds(us));
}

void DateTime::sleep_ms(uint64_t ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void DateTime::sleep_sec(uint64_t sec) {
	std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void DateTime::sleep_min(uint64_t min) {
	std::this_thread::sleep_for(std::chrono::minutes(min));
}

void DateTime::sleep_hour(uint64_t hour) {
	std::this_thread::sleep_for(std::chrono::hours(hour));
}

void DateTime::sleep_day(uint64_t day) {
	std::this_thread::sleep_for(std::chrono::seconds(day * 24 * 60 * 60));
}

}
}