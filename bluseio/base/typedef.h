#pragma once
#include <stdint.h>
#include <stddef.h>
#include <exception>

namespace bluseio {
namespace base {

/**
 * @brief 最大最小值
 * 
 * @tparam T 数据类型
 */
template <typename T = int>
struct MaxMin {
	T max_val;
	T min_val;
	T def_val;

	MaxMin() {}
	MaxMin(const T& max, const T& min, const T& def) : max_val(max), min_val(min), def_val(def) {
		if (min_val > max_val) {
			throw std::logic_error("min > max");
		}
		if (!in_range(def)) {
			throw std::logic_error("default val not in range");
		}
	}

	T value() {
		return def_val;
	}

	bool in_range(const T& v) {
		if (v < min_val || v > max_val) {
			return false;
		}
		return true;
	}

	bool overmax(const T& v) {
		return v > max_val;
	}

	bool overmin(const T& v) {
		return v < min_val;
	}
};

/**
 * @brief 数据buf
 * 
 */
struct DataBuf {
	char* addr;
	size_t capacity;	///< 容量
	size_t len;			///< 当前占用长度
	DataBuf() : addr(nullptr), capacity(0), len(0) {}
	DataBuf(char* ptr, size_t size, bool clr = true) : addr(ptr), capacity(size), len(0) {
		if (clr) {
			clear();
		}
	}
	DataBuf(DataBuf&& db) : addr(db.addr), capacity(db.capacity), len(db.len) {
		db.addr = nullptr;
		db.capacity = 0;
		db.len = 0;
	}
	DataBuf& operator=(DataBuf&& db) {
		addr = db.addr;
		capacity = db.capacity;
		len = db.len;
		db.addr = nullptr;
		db.capacity = 0;
		db.len = 0;

		return *this;
	}
	bool operator==(const DataBuf& db) const noexcept {
		if (capacity != db.capacity) {
			return false;
		}
		return 0 == ::memcmp(db.addr, addr, capacity);
	}
	bool data_equal(const DataBuf& db) const noexcept {
		if (len != db.len) {
			return false;
		}
		return 0 == ::memcmp(db.addr, addr, len);
	}
	void clear() {
		if (addr) {
			::memset(addr, 0, capacity);
			len = 0;
		}
	}
	void reset() {
		addr = nullptr;
		capacity = 0;
		len = 0;
	}
	bool valid() {
		return addr && capacity;
	}
	size_t size() {
		return len;
	}
	size_t free() {
		return capacity - len;
	}
	bool append(const char* data, size_t size) {
		if (size > (capacity - len)) {
			return false;
		}
		::memmove(addr + len, data, size);
		len += size;
		return true;
	}
	bool append(const std::string& data) {
		if (data.size() > (capacity - len)) {
			return false;
		}
		::memcpy(addr + len, data.data(), data.size());
		len += data.size();
		return true;
	}
	size_t data(char* data, size_t size) {
		if (!valid() || !data) {
			return 0;
		}
		::memmove(data, addr, std::min(size, len));
		return std::min(size, len);
	}
	size_t data(std::string& data) {
		if (!valid()) {
			return 0;
		}
		data = std::string(addr, len);
		return data.size();
	}
	std::string data() {
		if (!valid()) {
			return 0;
		}
		return std::string(addr, len);
	}
	~DataBuf() {
		reset();
	}
};

// 传输协议(32字节)
typedef struct {
	// 魔术字
	uint32_t magic;
	// 包编号
	uint32_t seq;
	// 校验码，checksum之后的crc32
	uint32_t checksum;
	// 版本号
	uint32_t version;
	// 正文的格式,json,xml,bin,other
	uint8_t body_format;
	// 是否是应答包
	uint8_t ack:1;
	// 是否加密
	uint8_t secret:1;
	// 是否扩展头部
	uint8_t header_extern:1;
	uint8_t res1:5;
	// 扩展头部长度
	uint8_t header_extern_len;
	// 预留
	uint8_t res[5];
	// 长度，整个包的长度(整个数据流)
	uint64_t content_len;
} fixed_header_t;

} // namespace base
} // namespace bluseio
