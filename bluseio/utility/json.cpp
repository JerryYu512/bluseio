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
 * @file json.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-01-30
 * 
 * @copyright MIT License
 * 
 */
#include <sstream>
#include "json.h"

namespace bluseio {
namespace utility {

Json::Json(JSON_TYPE type) {
	m_type = type;
	m_it = nullptr;
	switch (m_type) {
		case JSON_NULL:
			break;
		case JSON_BOOL:
			m_value.m_bool = false;
			break;
		case JSON_NUMBER:
			m_value.m_int = 0;
			break;
		case JSON_DOUBLE:
			m_value.m_double = 0.0;
			break;
		case JSON_STRING:
			m_value.m_string = new std::string;
			break;
		case JSON_ARRAY:
			m_value.m_array = new std::list<Json>;
			break;
		case JSON_OBJECT:
			m_value.m_object = new std::map<std::string, Json>;
			break;
	}
}

Json::~Json() {
	clear();
}

void Json::clear() {
	switch (m_type)
	{
	case JSON_NULL:
	case JSON_BOOL:
	case JSON_NUMBER:
	case JSON_DOUBLE:
		m_type = JSON_NULL;
		break;
	case JSON_STRING:
		if (m_value.m_string) {
			delete m_value.m_string;
			m_value.m_string = nullptr;
		}
		m_type = JSON_NULL;
		break;
	case JSON_ARRAY:
		if (m_value.m_array) {
			for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
				it->clear();
			}
			delete m_value.m_array;
			m_value.m_array = nullptr;
		}
		m_type = JSON_NULL;
		break;
	case JSON_OBJECT:
		if (m_value.m_object) {
			reset_travsing();
			for (auto &it : *m_value.m_object) {
				it.second.clear();
			}
			delete m_value.m_object;
			m_value.m_object = nullptr;
		}
		m_type = JSON_NULL;
		break;
	default:
		break;
	}

	m_type = JSON_NULL;
}

Json::Json(bool value) {
	m_type = JSON_BOOL;
	m_it = nullptr;
	m_value.m_bool = value;
}

Json::Json(int8_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(uint8_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(int16_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(uint16_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(int32_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(uint32_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(int64_t value) {
	m_type = JSON_NUMBER;
	m_it = nullptr;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(uint64_t value) {
	m_type = JSON_NUMBER;
	m_value.m_int = static_cast<uint64_t>(value);
}

Json::Json(float value) {
	m_type = JSON_DOUBLE;
	m_it = nullptr;
	m_value.m_double = static_cast<double>(value);
}

Json::Json(double value) {
	m_type = JSON_DOUBLE;
	m_it = nullptr;
	m_value.m_double = static_cast<double>(value);
}

Json::Json(const char* value) {
	m_type = JSON_STRING;
	m_it = nullptr;
	m_value.m_string = new std::string(value);
}

Json::Json(const std::string& value) {
	m_type = JSON_STRING;
	m_it = nullptr;
	m_value.m_string = new std::string(value);
}

Json::Json(const Json& other) {
	copy(other);
}

void Json::copy(const Json & other) {
	clear();

	m_type = other.m_type;
	m_it = nullptr;

	switch (m_type) {
		case JSON_NULL:
			break;
		case JSON_BOOL:
			m_value.m_bool = other.m_value.m_bool;
			break;
		case JSON_NUMBER:
			m_value.m_int = other.m_value.m_int;
			break;
		case JSON_DOUBLE:
			m_value.m_double = other.m_value.m_double;
			break;
		case JSON_STRING:
			m_value.m_string = new std::string(other.m_value.m_string->c_str());
			break;
		case JSON_ARRAY:
			m_value.m_array = new std::list<Json>(*other.m_value.m_array);
			break;
		case JSON_OBJECT:
			m_value.m_object = new std::map<std::string, Json>(*other.m_value.m_object);
			break;
	}
}

JSON_TYPE Json::type() const {
	return m_type;
}

Json & Json::operator = (const Json & other) {
	clear();
	copy(other);
	return *this;
}

bool Json::operator == (const Json & other) {
	if (m_type != other.m_type) {
		return false;
	}
	switch (m_type)
	{
		switch (m_type) {
			case JSON_NULL:
				return true;
			case JSON_BOOL:
				return m_value.m_bool == other.m_value.m_bool;
			case JSON_NUMBER:
				return m_value.m_int == other.m_value.m_int;
			case JSON_DOUBLE:
				return m_value.m_double == other.m_value.m_double;
			case JSON_STRING:
				return m_value.m_string == other.m_value.m_string;
			case JSON_ARRAY:
				return m_value.m_array == other.m_value.m_array;
			case JSON_OBJECT:
				return m_value.m_object == other.m_value.m_object;
			default:
				return false;
		}
	}
	return true;
}

bool Json::operator != (const Json & other) {
	return !(*this == other);
}

Json & Json::operator [] (int index) {
	if (JSON_ARRAY != m_type || index > (m_value.m_array->size() + 1))
	{
		throw std::logic_error("not array");
	}
	if (index < 0)
	{
		throw std::logic_error("index < 0");
	}
	if (index >= m_value.m_array->size())
	{
		throw std::logic_error("index over size");
	}
	auto it = (m_value.m_array)->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    return *it;
}

Json & Json::operator [] (const char * key) {
	std::string name = key;
    return (*this)[name];
}

Json & Json::operator [] (const std::string & key) {
	if (m_type != JSON_OBJECT)
    {
		throw std::logic_error("not object");
    }
	if (key.empty())
	{
		throw std::logic_error("key invalid");
	}
    return (*(m_value.m_object))[key];
}

Json::operator bool() {
	switch (m_type)
	{
		switch (m_type) {
			case JSON_NULL:
				return false;
			case JSON_BOOL:
				return m_value.m_bool;
			case JSON_NUMBER:
				return m_value.m_int;
			case JSON_DOUBLE:
				return m_value.m_double;
			case JSON_STRING:
				return m_value.m_string ? m_value.m_string->empty() : false;
			case JSON_ARRAY:
				return m_value.m_array ? m_value.m_array->empty() : false;
			case JSON_OBJECT:
				return m_value.m_object ? m_value.m_object->empty() : false;
			default:
				return false;
		}
	}
	return false;
}

Json::operator uint64_t() {
	if (JSON_NUMBER == m_type) {
		return m_value.m_int;
	}
	return 0;
}

Json::operator double() {
	if (JSON_DOUBLE == m_type) {
		return m_value.m_double;
	}
	return 0.0;
}

Json::operator std::string() {
	if (JSON_STRING == m_type && m_value.m_string) {
		return *m_value.m_string;
	}
	return "";
}

bool Json::is_null() const {
	return JSON_NULL == m_type;
}

bool Json::is_bool() const {
	return JSON_BOOL == m_type;
}

bool Json::is_number() const {
	return JSON_NUMBER == m_type || JSON_DOUBLE == m_type;
}

bool Json::is_int() const {
	return JSON_NUMBER == m_type;
}

bool Json::is_double() const {
	return JSON_DOUBLE == m_type;
}

bool Json::is_string() const {
	return JSON_STRING == m_type;
}

bool Json::is_array() const {
	return JSON_ARRAY == m_type;
}

bool Json::is_object() const {
	return JSON_OBJECT == m_type;
}

bool Json::as_bool() const {
	if (!is_bool()) {
		return false;
	}
	return m_value.m_bool;
}

uint64_t Json::as_int() const {
	if (!is_number()) {
		return 0;
	}
	return m_value.m_int;
}

double Json::as_double() const {
	if (!is_double()) {
		return 0.0;
	}
	return m_value.m_double;
}

std::string Json::as_string() const {
	if (!is_string()) {
		return "";
	}
	return *m_value.m_string;
}

Json::iterator Json::begin()
{
	if (!is_array() || !m_value.m_array) {
		throw std::logic_error("not array");
	}
	return (m_value.m_array)->begin();
}

Json::iterator Json::end()
{
	if (!is_array() || !m_value.m_array) {
		throw std::logic_error("not array");
	}
	return (m_value.m_array)->end();
}

// 遍历对象使用，依次返回key
bool Json::get_next_key(std::string& key) {
	if (!is_object() || !m_value.m_object) {
		return false;
	}

	if (!m_it) {
		m_it = new object_iterator;
		*m_it = m_value.m_object->begin();
		key = (*m_it)->first;
		return true;
	}

	if (*m_it != m_value.m_object->end()) {
		m_it++;
		key = (*m_it)->first;
		return true;
	} else {
		// 循环到最后时，重新循环
		*m_it = m_value.m_object->begin();
		key = (*m_it)->first;
		return true;
	}

	return false;
}

// 重置key的位置
void Json::reset_travsing(void) {
	if (m_it) {
		delete m_it;
		m_it = nullptr;
	}
}

std::string Json::format_str(bool format, int indent) const
{
    std::stringstream ss;
    switch (m_type)
    {
        case JSON_NULL:
            ss << "null";
            break;
        case JSON_BOOL:
            if (m_value.m_bool)
            {
                ss << "true";
            }
            else
            {
                ss << "false";
            }
            break;
        case JSON_NUMBER:
            ss << m_value.m_int;
            break;
        case JSON_DOUBLE:
            ss << m_value.m_double;
            break;
        case JSON_STRING:
            ss << "\"" << *(m_value.m_string) << "\"";
            break;
        case JSON_ARRAY:
            {
                ss << "[";
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                {
                    if (it != (m_value.m_array)->begin())
                    {
                        ss << ",";
                    }
                    ss << (*it).format_str(format, indent);
                }
                ss << "]";
            }
            break;
        case JSON_OBJECT:
            {
                ss << "{";
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                {
                    if (it != (m_value.m_object)->begin())
                    {
                        ss << ",";
                    }
                    ss << "\"" << it->first << "\":" << it->second.format_str(format, indent);
                }
                ss << "}";
            }
            break;
        default:
            break;
    }
    return ss.str();
}

} // namespace utility
} // namespace bluseio
