/**
 * MIT License
 * 
 * Copyright ? 2022 <Jerry.Yu>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * @file bloom_filter.hpp
 * @brief 布隆过滤器
 * @author Jerry.Yu (jerry.yu512@outlook.com)
 * @version 1.0.0
 * @date 2022-01-12
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <functional>
#include <bluseio/base/def.h>

namespace bluseio {
namespace utility {

using bloom_hash_fun_t = std::function<uint64_t(const void*key, size_t len)>;

 __attribute_used__ static uint64_t bl_sax_hash(const void *key, size_t len) {
    uint64_t h = 0;
    unsigned char *ptr = (unsigned char *)key;

    while (len) {
        h ^= (h << 5) + (h >> 2) + *ptr;
        ptr++;
        len--;
    }

    return h;
}

__attribute_used__ static uint64_t bl_sdbm_hash(const void *key, size_t len) {
    uint64_t h = 0;
    unsigned char *ptr = (unsigned char *)key;

    while (len) {
        h = *ptr + (h << 6) + (h << 16) - h;
        ptr++;
        len--;
    }

    return h;
}

/// @brief 布隆过滤器
/// @tparam size 布隆过滤器需要记录的数据量
template <unsigned int size>
class BloomFilter {
public:
	static_assert(size > 0);
	BloomFilter(const std::vector<bloom_hash_fun_t>& funs = std::vector<bloom_hash_fun_t>()) {
		static std::vector<bloom_hash_fun_t> def_hash_fun = {
			bl_sax_hash,
			bl_sdbm_hash,
		};

		if (hash_fun_.empty()) {
			hash_fun_ = def_hash_fun;
		}
		::memset(map_, 0, sizeof(map_));
	}

	~BloomFilter() {}

	/// @brief 添加记录
	/// @param key 
	/// @param len 
	/// @return 
	bool add(const void *key, size_t len) {
		if (!key || !len) {
			return false;
		}

		for (size_t n = 0; n < hash_fun_.size(); ++n) {
			BITMAP_SET(map_, hash_fun_[n](key, len) % (sizeof(map_) * CHAR_BIT));
		}

		return true;
	}

	/// @brief 检查记录
	/// @param key 
	/// @param len 
	/// @return 
	bool check(const void *key, size_t len) {
		if (!key || !len) {
			return true;
		}
		for (size_t n = 0; n < hash_fun_.size(); ++n) {
			// 只要有一个哈希函数标明未记录则标识未记录
			if (!BITMAP_GET(map_, hash_fun_[n](key, len) % (sizeof(map_) * CHAR_BIT))) {
				return false;
			}
		}

		return true;
	}

private:
	char map_[NUM_ALIGN(size, CHAR_BIT)];
	std::vector<bloom_hash_fun_t> hash_fun_;
};

}
}