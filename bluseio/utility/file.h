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
 * @file file.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-03-18
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <stdint.h>
#include <string>

namespace bluseio {
namespace utility {

class Path {
public:
	enum file_type {
		FILE_NORMAL,
		FILE_DIR,
		FILE_LINK,
		FILE_PIPE,
		FILE_NET,
		FILE_NONE = __UINT8_MAX__
	};

	static bool exsit(const char* filepath);
	static uint64_t size(const char* filepath);
	static file_type type(const char* filepath);
	static bool touch(const char* filepath);
	static bool mkdir(const char* filepath, bool p=false);
	static bool remove(const char* filepath, bool rf = false);
	/// @brief 文件修改时间
	/// @param filepath 
	/// @return 
	static int64 mtime(const char* filepath);
	static bool rename(const char* from, const char* to);
	static bool symlink(const char* from, const char* to);
};

class File {
public:
	enum seek_pos {
		SEEK_BEGIN,
		SEEK_CUR,
		SEEK_END,	
	};
public:
	// open mode:
	//   'r': read         open if exists
	//   'a': append       created if not exists
	//   'w': write        created if not exists, truncated if exists
	//   'm': modify       like 'w', but not truncated if exists
	//   '+': read/write   created if not exists

	/// @brief 打开文件，每次调用会关闭之前打开的文件
	/// @param filepath 
	/// @param mode 
	/// @return 
	File open(const char* filepath, char mode);
	void close();
	File(const char* filepath, char mode);
	File();
	~File();
	/// @brief 移动构造
	/// @param f 
	File(File&& f);
	File(const File& f) = delete;
	void operator=(const File&) = delete;
	void operator=(File&&) = delete;

	explicit operator bool() const;
	bool operator!() const {
		return !(bool)(*this);
	}
	const char* path();

	void seek(int64_t off, seek_pos pos=SEEK_BEGIN);
	size_t read(void *data, size_t len);
	std::string read(size_t len);
	std::string readall();
	size_t write(const void *data, size_t len);
	size_t write(const std::string& data);

	bool exsit(const char* filepath);
	uint64_t size(const char* filepath);
};

class CircleFile {
public:
	// open mode:
	//   'a': append       created if not exists
	//   'w': write        created if not exists, truncated if exists
	//   'm': modify       like 'w', but not truncated if exists
	CircleFile(const char* filepath, uint64_t size, char mode);
	~CircleFile();
	size_t write(void *data, size_t len);
	size_t write(const std::string& data);
};

}
}