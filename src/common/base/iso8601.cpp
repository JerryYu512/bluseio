#include "iso8601.h"
#include <string.h>
#include <hv/htime.h>

namespace bluseio {

#define EPOCH INT64_C(62135683200)  /* 1970-01-01T00:00:00 */
#define RDN_OFFSET INT64_C(62135683200)  /* 1970-01-01T00:00:00 */
#define MIN_SEC INT64_C(-62135596800) /* 0001-01-01T00:00:00 */
#define MAX_SEC INT64_C(253402300799) /* 9999-12-31T23:59:59 */

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

iso8601_t* iso8601_parse(iso8601_t* t, char* buf) {
    if (!t || !buf) {
        return NULL;
    }

    const unsigned char *cur, *end;
    unsigned char ch;
    uint16_t year, month, day, hour, min, sec;
    uint32_t rdn, sod, nsec;
    int16_t offset;
    size_t len = strlen(buf);

    /*
     *           1
     * 01234567890123456789
     * 2013-12-31T23:59:59Z
     */
    cur = (const unsigned char*)buf;
    if (len < 20 || cur[4] != '-' || cur[7] != '-' || cur[13] != ':' || cur[16] != ':') return NULL;

    ch = cur[10];
    if (!(ch == 'T' || ch == ' ' || ch == 't')) return NULL;

    if (_parse_4d(cur, 0, &year) || year < 1 || _parse_2d(cur, 5, &month) || month < 1 ||
        month > 12 || _parse_2d(cur, 8, &day) || day < 1 || day > 31 || _parse_2d(cur, 11, &hour) ||
        hour > 23 || _parse_2d(cur, 14, &min) || min > 59 || _parse_2d(cur, 17, &sec) || sec > 59)
        return nullptr;

    if (day > 28 && day > days_of_month(year, month)) return NULL;

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
        if (ndigits < 1 || ndigits > 9) return NULL;

        nsec *= Pow10[9 - ndigits];

        if (cur == end) return NULL;

        ch = *cur++;
    }

    if (!(ch == 'Z' || ch == 'z')) {
        /*
         *  01234
         * ±00:00
         */
        if (cur + 5 < end || !(ch == '+' || ch == '-') || cur[2] != ':') return NULL;

        if (_parse_2d(cur, 0, &hour) || hour > 23 || _parse_2d(cur, 3, &min) || min > 59) return NULL;

        offset = hour * 60 + min;
        if (ch == '-') offset *= -1;

        cur += 5;
    }

    if (cur != end) return NULL;

    t->sec = ((int64_t)rdn - 719163) * 86400 + sod - offset * 60;
    t->nsec = nsec;
    t->offset = offset;

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
	
} // namespace bluseio
