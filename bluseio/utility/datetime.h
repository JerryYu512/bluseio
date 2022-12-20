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
 * @file datetime.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-19
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <string.h>
#include <string>
#include <time.h>
#include <stdint.h>
#include <limits.h>

namespace bluseio {
namespace utility  {

#define SECONDS_PER_MINUTE  60
#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_DAY     86400   // 24*3600
#define SECONDS_PER_WEEK    604800  // 7*24*3600

#define IS_LEAP_YEAR(year) (((year)%4 == 0 && (year)%100 != 0) || (year)%400 == 0)

typedef struct {
    int year;
    int month;
    int day;
    // int wday;
    int hour;
    int min;
    int sec;
    int ms;
} date_t;

class DateTime {
public:
    /// @brief 默认当前时间
    DateTime();

    /// @brief 
    /// @param sec 绝对秒数
    /// @param us 微秒数
    /// @param timezone_offset_min 时区偏移的分钟数
    /// @param zone_name 时区名称
    DateTime(time_t sec, time_t us = 0, int16_t timezone_offset_min = INT16_MAX, const char* zone_name = nullptr);

    /// @brief 
    /// @param date 日期
    /// @param timezone_offset_min 时区偏移的分钟数
    /// @param zone_name 时区名称
    DateTime(const date_t& date, int16_t timezone_offset_min = INT16_MAX, const char* zone_name = nullptr);

    /// @brief 
    /// @param tm 日期
    /// @param timezone_offset_min 时区偏移的分钟数
    /// @param zone_name 时区名称
    DateTime(const struct tm& tm, int16_t timezone_offset_min = INT16_MAX, const char* zone_name = nullptr);

    DateTime(const DateTime& d) = default;

    /// @brief 时间是否相等，秒数和微秒数
    /// @param d 比较时间
    /// @return true-相等，false-不等
    bool operator==(const DateTime& d) const;

	/// @brief 时间是否有效
	/// @return true-有效，false-无效
	bool valid() const;

#define BIO_TIME_FMT            "%02d:%02d:%02d"
    /// @brief 日期字符串
    /// @return 
    std::string duration_str();
#define BIO_DATETIME_FMT        "%04d-%02d-%02d %02d:%02d:%02d"
    /// @brief 时间字符串
    /// @return 
    std::string datetime_str();
    /// @brief iso8601字符串
    /// @return 
    std::string iso8601_str(int precision = 0);
#define BIO_GMTIME_FMT          "%.3s, %02d %.3s %04d %02d:%02d:%02d GMT"
    /// @brief gmt时间字符串
    /// @return 
    std::string gmtime_str();
#define BIO_DATETIME_FMT_FILE     "%04d%02d%02d_%02d%02d%02d"
    /// @brief 文件字名符串
    /// @return 
    std::string file_str();
#define BIO_DATETIME_LOG_FMT    "%04d-%02d-%02d %02d:%02d:%02d.%03d"
    /// @brief 日志日期的格式
    /// @return 
    std::string logtime_str();
	/// @brief 类似命令date默认输出
	/// @return 
	std::string date_str();
	/// @brief 转换为秒
	/// @return 
	time_t seconds();

    date_t date(void);
    int16_t min_offset(void) const {
        return min_offset_;
    }
    std::string zonename(void) const {
        return zone_;
    }

    static clock_t clock();
	static uint32_t tick_ms(void);
	static uint64_t thrtime_us(void);
	static uint64_t now_us(void);
	static uint64_t now_ms(void);
    static time_t now_sec(void);

	static bool valid(const date_t& date);
	static bool valid(const struct tm& tm);

    static std::string utc_str(time_t sec, time_t us = 0, int16_t timezone_offset_min = INT16_MAX);
    static std::string local_str(time_t sec);
    static DateTime iso8601_parse(const char* str);

	static int days_of_month(int month, int year);
	static int month_atoi(const char* month);
	static const char* month_itoa(int month);
	static int weekday_atoi(const char* weekday);
	static const char* weekday_itoa(int weekday);

	static void sleep_us(uint64_t us);
	static void sleep_ms(uint64_t ms);
	static void sleep_sec(uint64_t sec);
	static void sleep_min(uint64_t min);
	static void sleep_hour(uint64_t hour);
	static void sleep_day(uint64_t day);

private:
	date_t date_;   ///< struct tm
    time_t sec_;    ///< Number of seconds since the epoch of 1970-01-01T00:00:00Z
    time_t usec_;
    time_t nsec_;   ///< Nanoseconds [0, 999999999]
    std::string zone_;
    int16_t min_offset_;   ///< Offset from UTC in minutes [-1439, 1439]
    int16_t dst_;
};

}
}