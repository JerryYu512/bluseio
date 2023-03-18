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
 * @file json.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-01-30
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <stdint.h>
#include <string>
#include <list>
#include <map>

namespace bluseio {
namespace utility {

typedef enum : int {
	JSON_NULL,
	JSON_BOOL,
	JSON_NUMBER,
	JSON_DOUBLE,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
} JSON_TYPE;

class Json {
public:
	Json(JSON_TYPE type = JSON_NULL);
	~Json();

	Json(bool value);
	Json(int8_t value);
	Json(uint8_t value);
	Json(int16_t value);
	Json(uint16_t value);
	Json(int32_t value);
	Json(uint32_t value);
	Json(int64_t value);
	Json(uint64_t value);
	Json(float value);
	Json(double value);
	Json(const char* value);
	Json(const std::string& value);
	Json(const Json& other);

	JSON_TYPE type() const;
	// 数组节点类型
	JSON_TYPE type(int index) const;
	// 对象节点类型
	JSON_TYPE type(const char* key) const;
	JSON_TYPE type(const std::string& key) const;

	Json & operator = (const Json & other);
    bool operator == (const Json & other);
    bool operator != (const Json & other);

	Json & operator [] (int index);
    Json & operator [] (const char * key);
    Json & operator [] (const std::string & key);

	operator bool();
	operator uint64_t();
	operator double();
	operator std::string();

	bool is_null() const;
	bool is_bool() const;
	bool is_number() const;
	bool is_int() const;
	bool is_double() const;
	bool is_string() const;
	bool is_array() const;
	bool is_object() const;

	// 数组节点类型判断
	bool is_null(int index) const;
	bool is_bool(int index) const;
	bool is_number(int index) const;
	bool is_int(int index) const;
	bool is_double(int index) const;
	bool is_string(int index) const;
	bool is_array(int index) const;
	bool is_object(int index) const;

	// 对象节点类型判断
	bool is_null(const char* key) const;
	bool is_bool(const char* key) const;
	bool is_number(const char* key) const;
	bool is_int(const char* key) const;
	bool is_double(const char* key) const;
	bool is_string(const char* key) const;
	bool is_array(const char* key) const;
	bool is_object(const char* key) const;

	bool is_null(const std::string& key) const;
	bool is_bool(const std::string& key) const;
	bool is_number(const std::string& key) const;
	bool is_int(const std::string& key) const;
	bool is_double(const std::string& key) const;
	bool is_string(const std::string& key) const;
	bool is_array(const std::string& key) const;
	bool is_object(const std::string& key) const;

	bool as_bool() const;
	uint64_t as_int() const;
	double as_double() const;
	std::string as_string() const;

	// 数组，对象为空或者null
	bool empty() const;

	void clear();

	bool parse(const char* str);
	bool parse(const std::string& str);
	// 输出为字符串
	std::string format_str(bool format = false, int indent = 2) const;
	
	// 数组或对象的值数量
	size_t size(void) const;

	bool has(int index) const;
	bool has(const char* key) const;
	bool has(const std::string& key) const;

	bool remove(int index);
	bool remove(const char* key);
	bool remove(const std::string& key);

	// 添加到数组最前边
	bool add_as_first(bool value);
	bool add_as_first(int8_t value);
	bool add_as_first(uint8_t value);
	bool add_as_first(int16_t value);
	bool add_as_first(uint16_t value);
	bool add_as_first(int32_t value);
	bool add_as_first(uint32_t value);
	bool add_as_first(int64_t value);
	bool add_as_first(uint64_t value);
	bool add_as_first(float value);
	bool add_as_first(double value);
	bool add_as_first(const char* value);
	bool add_as_first(const std::string& value);
	bool add_as_first(Json& json);
	bool add_as_first_empty_object(void);
	bool add_as_first_empty_array(void);

	bool append(bool value);
	bool append(int8_t value);
	bool append(uint8_t value);
	bool append(int16_t value);
	bool append(uint16_t value);
	bool append(int32_t value);
	bool append(uint32_t value);
	bool append(int64_t value);
	bool append(uint64_t value);
	bool append(float value);
	bool append(double value);
	bool append(const char* value);
	bool append(const std::string& value);
	bool append(Json& value);
	bool append_empty_object(void);
	bool append_empty_array(void);

	bool insert(int index, bool value);
	bool insert(int index, int8_t value);
	bool insert(int index, uint8_t value);
	bool insert(int index, int16_t value);
	bool insert(int index, uint16_t value);
	bool insert(int index, int32_t value);
	bool insert(int index, uint32_t value);
	bool insert(int index, int64_t value);
	bool insert(int index, uint64_t value);
	bool insert(int index, float value);
	bool insert(int index, double value);
	bool insert(int index, const char* value);
	bool insert(int index, const std::string& value);
	bool insert(int index, Json& value);
	bool insert_empty_object(int index);
	bool insert_empty_array(int index);

	bool replace(int index, bool value);
	bool replace(int index, int8_t value);
	bool replace(int index, uint8_t value);
	bool replace(int index, int16_t value);
	bool replace(int index, uint16_t value);
	bool replace(int index, int32_t value);
	bool replace(int index, uint32_t value);
	bool replace(int index, int64_t value);
	bool replace(int index, uint64_t value);
	bool replace(int index, float value);
	bool replace(int index, double value);
	bool replace(int index, const char* value);
	bool replace(int index, const std::string& value);
	bool replace(int index, Json& value);
	bool replace_empty_object(int index);
	bool replace_empty_array(int index);

	bool get(int index, bool& value);
	bool get(int index, int8_t& value);
	bool get(int index, uint8_t& value);
	bool get(int index, int16_t& value);
	bool get(int index, uint16_t& value);
	bool get(int index, int32_t& value);
	bool get(int index, uint32_t& value);
	bool get(int index, int64_t& value);
	bool get(int index, uint64_t& value);
	bool get(int index, float& value);
	bool get(int index, double& value);
	bool get(int index, std::string& value);
	bool get(int index, Json& value);

	template <typename T>
	T get(int index) {
		T value;
		get(index, value);
		return value;
	}

	// object add
	bool add(const char* key, bool value);
	bool add(const char* key, int8_t value);
	bool add(const char* key, uint8_t value);
	bool add(const char* key, int16_t value);
	bool add(const char* key, uint16_t value);
	bool add(const char* key, int32_t value);
	bool add(const char* key, uint32_t value);
	bool add(const char* key, int64_t value);
	bool add(const char* key, uint64_t value);
	bool add(const char* key, float value);
	bool add(const char* key, double value);
	bool add(const char* key, const char* value);
	bool add(const char* key, const std::string& value);
	bool add(const char* key, Json& value);
	bool add_empty_object(const char* key);
	bool add_empty_array(const char* key);

	// 遍历数组
	typedef std::list<Json>::iterator iterator;
    iterator begin();
    iterator end();

	bool add(const std::string& key, bool value);
	bool add(const std::string& key, int8_t value);
	bool add(const std::string& key, uint8_t value);
	bool add(const std::string& key, int16_t value);
	bool add(const std::string& key, uint16_t value);
	bool add(const std::string& key, int32_t value);
	bool add(const std::string& key, uint32_t value);
	bool add(const std::string& key, int64_t value);
	bool add(const std::string& key, uint64_t value);
	bool add(const std::string& key, float value);
	bool add(const std::string& key, double value);
	bool add(const std::string& key, const char* value);
	bool add(const std::string& key, const std::string& value);
	bool add(const std::string& key, Json& value);
	bool add_empty_object(const std::string& key);
	bool add_empty_array(const std::string& key);

	bool replace(const char* key, bool value);
	bool replace(const char* key, int8_t value);
	bool replace(const char* key, uint8_t value);
	bool replace(const char* key, int16_t value);
	bool replace(const char* key, uint16_t value);
	bool replace(const char* key, int32_t value);
	bool replace(const char* key, uint32_t value);
	bool replace(const char* key, int64_t value);
	bool replace(const char* key, uint64_t value);
	bool replace(const char* key, float value);
	bool replace(const char* key, double value);
	bool replace(const char* key, const char* value);
	bool replace(const char* key, const std::string& value);
	bool replace(const char* key, Json& value);
	bool replace_empty_object(const char* key);
	bool replace_empty_array(const char* key);

	bool replace(const std::string& key, bool value);
	bool replace(const std::string& key, int8_t value);
	bool replace(const std::string& key, uint8_t value);
	bool replace(const std::string& key, int16_t value);
	bool replace(const std::string& key, uint16_t value);
	bool replace(const std::string& key, int32_t value);
	bool replace(const std::string& key, uint32_t value);
	bool replace(const std::string& key, int64_t value);
	bool replace(const std::string& key, uint64_t value);
	bool replace(const std::string& key, float value);
	bool replace(const std::string& key, double value);
	bool replace(const std::string& key, const char* value);
	bool replace(const std::string& key, const std::string& value);
	bool replace(const std::string& key, Json& value);
	bool replace_empty_object(const std::string& key);
	bool replace_empty_array(const std::string& key);

	bool get(const char* key, bool& value);
	bool get(const char* key, int8_t& value);
	bool get(const char* key, uint8_t& value);
	bool get(const char* key, int16_t& value);
	bool get(const char* key, uint16_t& value);
	bool get(const char* key, int32_t& value);
	bool get(const char* key, uint32_t& value);
	bool get(const char* key, int64_t& value);
	bool get(const char* key, uint64_t& value);
	bool get(const char* key, float& value);
	bool get(const char* key, double& value);
	bool get(const char* key, std::string& value);
	bool get(const char* key, Json& value);

	template <typename T>
	T get(const char* key) {
		T value;
		get(key, value);
		return value;
	}

	bool get(const std::string& key, bool& value);
	bool get(const std::string& key, int8_t& value);
	bool get(const std::string& key, uint8_t& value);
	bool get(const std::string& key, int16_t& value);
	bool get(const std::string& key, uint16_t& value);
	bool get(const std::string& key, int32_t& value);
	bool get(const std::string& key, uint32_t& value);
	bool get(const std::string& key, int64_t& value);
	bool get(const std::string& key, uint64_t& value);
	bool get(const std::string& key, float& value);
	bool get(const std::string& key, double& value);
	bool get(const std::string& key, std::string& value);
	bool get(const std::string& key, Json& value);

	template <typename T>
	T get(const std::string& key) {
		T value;
		get(key, value);
		return value;
	}

	// 遍历对象使用，依次返回key
	bool get_next_key(std::string& key);
	// 重置key的位置
	void reset_travsing(void);

	// 获取错误信息
	std::string get_error_msg() const;

private:
    void copy(const Json & other);

private:
    typedef union {
        bool m_bool;
        uint64_t m_int;
        double m_double;
        std::string* m_string;
        std::list<Json>* m_array;
        // std::map<uint32_t, Json>* m_array2;
        std::map<std::string, Json>* m_object;
    } JSON_Value;

    JSON_TYPE m_type;
    JSON_Value m_value;

	typedef std::map<std::string, Json>::iterator object_iterator;
	object_iterator *m_it;
};
	
} // namespace utility
} // namespace bluseio
