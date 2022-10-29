#pragma once
#include <stdint.h>

namespace bluseio {

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

} // namespace bluseio
