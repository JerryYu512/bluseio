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
 * @file topic.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-10-30
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <string>
#include <bluseio/ecode/ecode_def.h>

namespace bluseio {

namespace mq {

/// @brief 系统topic
class SysTopic {
public:
	SysTopic() = delete;
	~SysTopic() = delete;

	/// @brief 查询系统信息
	/// @param url 
	/// @param topic 
	/// @param result 
	/// @return 
	bio_err_t query(const std::string& url, const std::string& topic, std::string& result);
};

/// @brief 客户端
class Client {
public:
	Client() = delete;
	~Client() = delete;

	/// @brief 设置消息的长度限制
	/// @warning 后设置的会覆盖前边的
	/// @param url 
	/// @param send 
	/// @param recv 
	/// @return 
	bio_err_t set_msg_size(const std::string& url, uint32_t send, uint32_t recv);
	bio_err_t set_msg_size(const std::string& url, const std::string& topic, uint32_t send, uint32_t recv);
};
	
} // namespace mq

} // namespace bluseio
