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
 * @file global_var.h
 * @brief 全局变量
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-20
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <stdint.h>
#include <string>
#include <vector>

namespace bluseio {
namespace utility {

enum GVAR_TYPE {
	GVAR_BOOL,
	GVAR_INT8,
	GVAR_UINT8,
	GVAR_INT16,
	GVAR_UINT16,
	GVAR_INT32,
	GVAR_UINT32,
	GVAR_INT64,
	GVAR_UINT64,
	GVAR_FLOAT,
	GVAR_DOUBLE,
	GVAR_STRING,
};

std::vector<std::string> gvar_init(int argc, const char** argv);
void gvar_init(const char* cfgfilepath);
std::string set_gvalue(const std::string& name, const std::string& value);

namespace gxx {
	void add_gvar(
		GVAR_TYPE type, const char* name, const char* value, const char* help,
		const char* file, int line, void* addr, const char* alias
	);
}

/// 全局变量声明
#define GVAR_DEC(type, name) extern type GVAR_##name
/// 全局变量定义
#define GVAR_DEF(type, name, id, var, help, ...) \
	type GVAR_##name = []() {\
	bluseio::utility::gxx::add_gvar(id, #name, #var, help, __FILE__, __LINE__, &GVAR_##name, ""#__VA_ARGS__); \
	return var;\
	}()

/// 变量声明
#define GDEC_bool(name) GVAR_DEC(bool, name)
#define GDEC_int8(name) GVAR_DEC(int8_t, name)
#define GDEC_uint8(name) GVAR_DEC(uint8_t, name)
#define GDEC_int16(name) GVAR_DEC(int16_t, name)
#define GDEC_uint16(name) GVAR_DEC(uint16_t, name)
#define GDEC_int32(name) GVAR_DEC(int32_t, name)
#define GDEC_uint32(name) GVAR_DEC(uint32_t, name)
#define GDEC_int64(name) GVAR_DEC(int64_t, name)
#define GDEC_uint64(name) GVAR_DEC(uint64_t, name)
#define GDEC_float(name) GVAR_DEC(float, name)
#define GDEC_double(name) GVAR_DEC(double, name)
#define GDEC_string(name) GVAR_DEC(std::string, name)

/// 定义
#define GDEF_bool(name, var, help, ...) GVAR_DEF(bool, name, bluseio::utility::GVAR_BOOL, var, help, __VA_ARGS__)
#define GDEF_int8(name, var, help, ...) GVAR_DEF(int8_t, name, bluseio::utility::GVAR_INT8, var, help, __VA_ARGS__)
#define GDEF_uint8(name, var, help, ...) GVAR_DEF(uint8_t, name, bluseio::utility::GVAR_UINT8, var, help, __VA_ARGS__)
#define GDEF_int16(name, var, help, ...) GVAR_DEF(int16_t, name, bluseio::utility::GVAR_INT16, var, help, __VA_ARGS__)
#define GDEF_uint16(name, var, help, ...) GVAR_DEF(uint16_t, name, bluseio::utility::GVAR_UINT16, var, help, __VA_ARGS__)
#define GDEF_int32(name, var, help, ...) GVAR_DEF(int32_t, name, bluseio::utility::GVAR_INT32, var, help, __VA_ARGS__)
#define GDEF_uint32(name, var, help, ...) GVAR_DEF(uint32_t, name, bluseio::utility::GVAR_UINT32, var, help, __VA_ARGS__)
#define GDEF_int64(name, var, help, ...) GVAR_DEF(int64_t, name, bluseio::utility::GVAR_INT64, var, help, __VA_ARGS__)
#define GDEF_uint64(name, var, help, ...) GVAR_DEF(uint64_t, name, bluseio::utility::GVAR_UINT64, var, help, __VA_ARGS__)
#define GDEF_float(name, var, help, ...) GVAR_DEF(float, name, var, bluseio::utility::GVAR_FLOAT, help, __VA_ARGS__)
#define GDEF_double(name, var, help, ...) GVAR_DEF(double, name, bluseio::utility::GVAR_DOUBLE, var, help, __VA_ARGS__)
#define GDEF_string(name, var, help, ...) GVAR_DEF(std::string, name, bluseio::utility::GVAR_STRING, var, help, __VA_ARGS__)

GDEC_string(config);
GDEC_string(help);
GDEC_string(version);

}
}