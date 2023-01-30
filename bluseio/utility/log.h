/**
 * MIT License
 * 
 * Copyright © 2023 <Jerry.Yu>.
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
 * @file log.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-01-28
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include <functional>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

namespace bluseio {
namespace utility {

#define CLR_CLR         "\033[0m"       /* 恢复颜色 */
#define CLR_BLACK       "\033[30m"      /* 黑色字 */
#define CLR_RED         "\033[31m"      /* 红色字 */
#define CLR_GREEN       "\033[32m"      /* 绿色字 */
#define CLR_YELLOW      "\033[33m"      /* 黄色字 */
#define CLR_BLUE        "\033[34m"      /* 蓝色字 */
#define CLR_PURPLE      "\033[35m"      /* 紫色字 */
#define CLR_SKYBLUE     "\033[36m"      /* 天蓝字 */
#define CLR_WHITE       "\033[37m"      /* 白色字 */

#define CLR_BLK_WHT     "\033[40;37m"   /* 黑底白字 */
#define CLR_RED_WHT     "\033[41;37m"   /* 红底白字 */
#define CLR_GREEN_WHT   "\033[42;37m"   /* 绿底白字 */
#define CLR_YELLOW_WHT  "\033[43;37m"   /* 黄底白字 */
#define CLR_BLUE_WHT    "\033[44;37m"   /* 蓝底白字 */
#define CLR_PURPLE_WHT  "\033[45;37m"   /* 紫底白字 */
#define CLR_SKYBLUE_WHT "\033[46;37m"   /* 天蓝底白字 */
#define CLR_WHT_BLK     "\033[47;30m"   /* 白底黑字 */

// XXX(id, str, clr)
#define LOG_LEVEL_MAP(XXX) \
    XXX(LOG_LEVEL_DEBUG, "DEBUG", CLR_WHITE)     \
    XXX(LOG_LEVEL_INFO,  "INFO ", CLR_GREEN)     \
    XXX(LOG_LEVEL_WARN,  "WARN ", CLR_YELLOW)    \
    XXX(LOG_LEVEL_ERROR, "ERROR", CLR_RED)       \
    XXX(LOG_LEVEL_FATAL, "FATAL", CLR_RED_WHT)

typedef enum {
    LOG_LEVEL_VERBOSE = 0,
#define XXX(id, str, clr) id,
    LOG_LEVEL_MAP(XXX)
#undef  XXX
    LOG_LEVEL_SILENT
} log_level_e;

#ifndef LOG_TAG
#define LOG_TAG "DLOG"
#endif

#define DEFAULT_LOG_FILE            "logfile"
#define DEFAULT_LOG_LEVEL           LOG_LEVEL_INFO
#define DEFAULT_LOG_FORMAT          "%y-%m-%d %H:%M:%S.%z %L %s"
#define DEFAULT_LOG_REMAIN_DAYS     1
#define DEFAULT_LOG_MAX_BUFSIZE     (1<<14)  // 16k
#define DEFAULT_LOG_MAX_FILESIZE    (1<<24)  // 16M

// 过滤
using logger_filter_cb_t = std::function<bool(log_level_e lv, const char* tag, const char* filename, int line, const char* funname, const char* buf, int len)>;
// 重定向
using logger_direct_cb_t = std::function<bool(log_level_e lv, const char* tag, const char* filename, int line, const char* funname, const char* buf, int len)>;

typedef struct logger_info_s logger_info_t;

class Logger {

public:
	Logger(log_level_e lv = DEFAULT_LOG_LEVEL);
	~Logger();

	// def en
	void set_stdout_enable(bool en);
	void set_stdout_level(log_level_e lv);
	void set_max_bufsize(uint32_t size);
	void set_color_enable(bool en);
	void set_file(const char* filepath);
	void set_file_enable(bool en);
	void set_file_level(log_level_e lv);
	void set_max_filesize(uint32_t size);
	void set_remain_days(uint32_t days);
	void set_enable_fsync(bool en);
	void fsync(void);
	const char* cur_filename(void);

	void set_filter_cb(logger_filter_cb_t cb);
	void set_direct_cb(logger_direct_cb_t cb);

	int print(log_level_e lv, const char* tag, const char* filename, int line, const char* funname, const char* fmt, ...) __attribute__((format(printf, 7, 8)));

	static Logger& default_logger() {
		static Logger log;
		return log;
	}

private:
	logger_info_t *loger_;
};

#define blog_debug(fmt, ...) bluseio::utility::Logger::default_logger().print(bluseio::utility::LOG_LEVEL_DEBUG, LOG_TAG, __FILE__, __LINE__, __FUNCTION__, fmt, ## __VA_ARGS__)
#define blog_info(fmt, ...) bluseio::utility::Logger::default_logger().print(bluseio::utility::LOG_LEVEL_INFO, LOG_TAG, __FILE__, __LINE__, __FUNCTION__, fmt, ## __VA_ARGS__)
#define blog_warn(fmt, ...) bluseio::utility::Logger::default_logger().print(bluseio::utility::LOG_LEVEL_WARN, LOG_TAG, __FILE__, __LINE__, __FUNCTION__, fmt, ## __VA_ARGS__)
#define blog_error(fmt, ...) bluseio::utility::Logger::default_logger().print(bluseio::utility::LOG_LEVEL_ERROR, LOG_TAG, __FILE__, __LINE__, __FUNCTION__, fmt, ## __VA_ARGS__)
#define blog_fatal(fmt, ...) bluseio::utility::Logger::default_logger().print(bluseio::utility::LOG_LEVEL_FATAL, LOG_TAG, __FILE__, __LINE__, __FUNCTION__, fmt, ## __VA_ARGS__)

}
} // namespace bluseio
