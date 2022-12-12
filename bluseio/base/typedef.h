#pragma once
#include <stdint.h>
#include <stddef.h>

namespace bluseio {
namespace base {

/**
 * @brief 最大最小值
 * 
 * @tparam T 数据类型
 */
template <typename T = int>
typedef struct {
	T max_val;
	T min_val;
	T def_val;
} MaxMin;

/**
 * @brief 数据buf
 * 
 */
typedef struct {
	char* addr;
	size_t capacity;	///< 容量
	size_t len;
} data_buf_t;

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
