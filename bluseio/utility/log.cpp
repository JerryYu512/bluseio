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
 * @file log.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-01-28
 * 
 * @copyright MIT License
 * 
 */

#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <mutex>
#include <stdarg.h>
#include <sys/time.h>

namespace bluseio {

namespace utility {

#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_DAY     86400   // 24*3600
#define SECONDS_PER_WEEK    604800  // 7*24*3600;

struct logger_info_s {
	logger_filter_cb_t filter;
	logger_direct_cb_t direct;
	uint32_t bufsize;
	char* buf;

	log_level_e level;

	bool stdout_enable;
	bool color;

	char filepath[256];
	bool file_write_enable;
	log_level_e file_write_level;
	uint32_t max_filesize;
	time_t remain_days;
	bool enable_fsync;
	FILE* fp;
	char cur_logfile[256];
	time_t last_logfile_ts;
	int can_write_cnt;

	int gmtoff;
	std::mutex mtx;
};

static void logger_set_file(logger_info_t* logger, const char* filepath) {
    strncpy(logger->filepath, filepath, sizeof(logger->filepath) - 1);
    // remove suffix .log
    char* suffix = strrchr(logger->filepath, '.');
    if (suffix && strcmp(suffix, ".log") == 0) {
        *suffix = '\0';
    }
}

Logger::Logger(log_level_e lv) {
	loger_ = new logger_info_t;

	loger_->filter = nullptr;
	loger_->direct = nullptr;
	loger_->bufsize = DEFAULT_LOG_MAX_BUFSIZE;

	loger_->level = lv;

	loger_->stdout_enable = true;
	loger_->color = false;

	memset(loger_->filepath, 0, sizeof(loger_->filepath));
	logger_set_file(loger_, DEFAULT_LOG_FILE);
	loger_->file_write_enable = false;
	loger_->file_write_level = lv;
	loger_->max_filesize = DEFAULT_LOG_MAX_FILESIZE;
	loger_->remain_days = DEFAULT_LOG_REMAIN_DAYS;
	loger_->enable_fsync = true;
	loger_->last_logfile_ts = 0;
	loger_->can_write_cnt = -1;

    time_t ts = time(NULL);
    struct tm* local_tm = localtime(&ts);
    int local_hour = local_tm->tm_hour;
    struct tm* gmt_tm = gmtime(&ts);
    int gmt_hour = gmt_tm->tm_hour;
    loger_->gmtoff = (local_hour - gmt_hour) * SECONDS_PER_HOUR;
}

Logger::~Logger() {
	if (loger_) {
		loger_->mtx.lock();
		if (loger_->buf) {
			free(loger_->buf);
			loger_->buf = nullptr;
		}
		if (loger_->fp) {
			fclose(loger_->fp);
			loger_->fp = nullptr;
		}
		loger_->mtx.unlock();
	}
}

// def en
void Logger::set_stdout_enable(bool en) {
	loger_->stdout_enable = true;
}

void Logger::set_stdout_level(log_level_e lv) {
	loger_->level = lv;
}

void Logger::set_max_bufsize(uint32_t size) {
	loger_->mtx.lock();
	loger_->bufsize = size;
	if (loger_->buf) {
		loger_->buf = (char*)realloc(loger_->buf, size);
	} else {
		loger_->buf = (char*)malloc(size);
	}
	loger_->mtx.unlock();
}

void Logger::set_color_enable(bool en) {
	loger_->color = true;
}

void Logger::set_file(const char* filepath) {
	logger_set_file(loger_, filepath);
}

void Logger::set_file_enable(bool en) {
	loger_->file_write_enable = en;
}

void Logger::set_file_level(log_level_e lv) {
	loger_->file_write_level = lv;
}

void Logger::set_max_filesize(uint32_t size) {
	loger_->max_filesize = size;
}

void Logger::set_remain_days(uint32_t day) {
	loger_->remain_days = day;
}

void Logger::set_enable_fsync(bool en) {
	loger_->enable_fsync = en;
}

void Logger::fsync(void) {
	loger_->mtx.lock();
	if (loger_->fp) {
		fflush(loger_->fp);
	}
	loger_->mtx.unlock();
}

const char* Logger::cur_filename(void) {
	return loger_->cur_logfile;
}

static void logfile_name(const char* filepath, time_t ts, char* buf, int len) {
    struct tm* tm = localtime(&ts);
	snprintf(buf, len, "%s.%04d%02d%02d.log",
			 filepath,
			 tm->tm_year + 1900,
			 tm->tm_mon + 1,
			 tm->tm_mday);
}

static FILE* logfile_shift(logger_info_t* logger) {
	time_t ts_now = time(NULL);
	int interval_days = logger->last_logfile_ts == 0 ? 0 : (ts_now + logger->gmtoff) / SECONDS_PER_DAY - (logger->last_logfile_ts + logger->gmtoff) / SECONDS_PER_DAY;
	if (logger->fp == NULL || interval_days > 0) {
		// close old logfile
		if (logger->fp) {
			fclose(logger->fp);
			logger->fp = NULL;
		} else {
			interval_days = 30;
		}

		if (logger->remain_days >= 0) {
			char rm_logfile[256] = {0};
			if (interval_days >= logger->remain_days) {
				// remove [today-interval_days, today-remain_days] logfile
				for (int i = interval_days; i >= logger->remain_days; --i) {
					time_t ts_rm = ts_now - i * SECONDS_PER_DAY;
					logfile_name(logger->filepath, ts_rm, rm_logfile, sizeof(rm_logfile));
					remove(rm_logfile);
				}
			} else {
				// remove today-remain_days logfile
				time_t ts_rm = ts_now - logger->remain_days * SECONDS_PER_DAY;
				logfile_name(logger->filepath, ts_rm, rm_logfile, sizeof(rm_logfile));
				remove(rm_logfile);
			}
		}
	}

	// open today logfile
	if (logger->fp == NULL) {
		logfile_name(logger->filepath, ts_now, logger->cur_logfile, sizeof(logger->cur_logfile));
		logger->fp = fopen(logger->cur_logfile, "a");
		logger->last_logfile_ts = ts_now;
	}

	// NOTE: estimate can_write_cnt to avoid frequent fseek/ftell
	if (logger->fp && --logger->can_write_cnt < 0) {
		fseek(logger->fp, 0, SEEK_END);
		long filesize = ftell(logger->fp);
		if (filesize > logger->max_filesize) {
			fclose(logger->fp);
			logger->fp = NULL;
			// ftruncate
			logger->fp = fopen(logger->cur_logfile, "w");
			// reopen with O_APPEND for multi-processes
			if (logger->fp)
			{
				fclose(logger->fp);
				logger->fp = fopen(logger->cur_logfile, "a");
			}
		} else {
			logger->can_write_cnt = (logger->max_filesize - filesize) / logger->bufsize;
		}
	}

	return logger->fp;
}

static void logfile_write(logger_info_t* logger, const char* buf, int len) {
    FILE* fp = logfile_shift(logger);
    if (fp) {
        fwrite(buf, 1, len, fp);
        if (logger->enable_fsync) {
            fflush(fp);
        }
    }
}

void Logger::set_filter_cb(logger_filter_cb_t cb) {
	loger_->filter = cb;
}

void Logger::set_direct_cb(logger_direct_cb_t cb) {
	loger_->direct = cb;
}

int Logger::print(log_level_e lv, const char* tag, const char* filename, int line, const char* funname, const char* fmt, ...) {
	// 输出未使能
	if (!loger_->stdout_enable && !loger_->file_write_enable) {
		return -10;
	}

	// 等级全过滤
	if (loger_->level > lv && loger_->file_write_level > lv) {
		return -20;
	}

	if (LOG_LEVEL_SILENT == loger_->level && LOG_LEVEL_SILENT == loger_->file_write_level) {
		return -30;
	}

	// 非法的等级
	if (LOG_LEVEL_SILENT == lv || LOG_LEVEL_VERBOSE == lv) {
		return -40;
	}

    int year,month,day,hour,min,sec,us;
    struct timeval tv;
    struct tm* tm = NULL;
    gettimeofday(&tv, NULL);
    time_t tt = tv.tv_sec;
    tm = localtime(&tt);
    year     = tm->tm_year + 1900;
    month    = tm->tm_mon  + 1;
    day      = tm->tm_mday;
    hour     = tm->tm_hour;
    min      = tm->tm_min;
    sec      = tm->tm_sec;
    us       = tv.tv_usec;

    const char* pcolor = "";
    const char* plevel = "";
	size_t color_len = 0;
#define XXX(id, str, clr) \
    case id: plevel = str; pcolor = clr; break;

    switch (lv) {
        LOG_LEVEL_MAP(XXX)
    }
#undef XXX

	color_len = strlen(pcolor);

	std::unique_lock<std::mutex> lck(loger_->mtx);

	if (!loger_->buf) {
		loger_->buf = (char*)malloc(loger_->bufsize);
	}

	if (!loger_->buf) {
		return -50;
	}

    char* buf = loger_->buf;
    uint32_t bufsize = loger_->bufsize;
    int len = 0;

	memset(buf, 0, bufsize);

    if (loger_->color) {
        len = snprintf(buf, bufsize, "%s", pcolor);
    }

	len += snprintf(buf + len, bufsize - len, "%04d-%02d-%02d %02d:%02d:%02d.%03d %s [%s][%s:%d:%s] ",
		year, month, day, hour, min, sec, us/1000,
		plevel, tag, filename, line, funname);

	va_list ap;
	va_start(ap, fmt);
	len += vsnprintf(buf + len, bufsize - len, fmt, ap);
	va_end(ap);

    if (loger_->color) {
        len += snprintf(buf + len, bufsize - len, "%s", CLR_CLR);
	}

	if (loger_->filter) {
		bool ret = loger_->filter(lv, tag, filename, line, funname, buf + color_len, len - color_len - strlen(CLR_CLR));
		if (ret) {
			return len - color_len - strlen(CLR_CLR);
		}
	}

	if (loger_->direct) {
		bool ret = loger_->direct(lv, tag, filename, line, funname, buf + color_len, len - color_len - strlen(CLR_CLR));
		if (ret) {
			return len - color_len - strlen(CLR_CLR);
		}
	}

	if (loger_->stdout_enable && loger_->level <= lv && LOG_LEVEL_SILENT != loger_->level) {
		printf("%s", buf);
	}

	if (loger_->file_write_enable && loger_->file_write_level <= lv && LOG_LEVEL_SILENT != loger_->file_write_level) {
		logfile_write(loger_, buf + color_len, len - color_len - strlen(CLR_CLR));
	}

	return len;
}

} // namespace utility
} // namespace bluseio
