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
 * @file msg.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-10-30
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <sys/uio.h>
#include <bluseio/base/typedef.h>

namespace bluseio {
	
namespace mq {

typedef struct msg_time_s {
	time_t sec;
	time_t mill;
} msg_time_t;

typedef enum {
	MSG_QOS1,	// 最多一次
	MSG_QOS2,	// 至少一次
	MSG_QOS3,	// 仅一次
} MsgQos;

/**
 * @brief 消息接收规则
 * 
 */
typedef enum {
	RECV_NOT_LIMIT = 0,	///< 不限制接收者
	RECV_LEAST_ONE = 1, ///< 至少一个
	RECV_ONLY_THEM = 2, ///< 只能这些接收者接收
	RECV_LEAST_THEM = 3, ///< 保证这些接收者
	RECV_NOT_THEM = 4,	///< 不让这些接收者接收
} MsgRecvRule;

typedef struct {
	// 话题
	std::string topic;
	// 消息唯一标识
	std::string uuid;
	// QOS
	MsgQos qos;
	// 消费次数
	uint32_t cust_cnt;
	// 失败次数
	uint32_t fail_cnt;
	// 请求还是应答消息
	uint8_t req_res;
	// 发送者
	std::string sender;
	// 接收者
	std::vector<std::string> receivers;
	// receivers不为空时有效
	MsgRecvRule recv_rule;
	// 接收成功队列->次数
	std::map<std::string, uint32_t> recv_succ;
	// 接收失败队列->次数
	std::map<std::string, uint32_t> recv_fail;
	// 推送的时间
	msg_time_t send_time;
	// 有效期，0-未指定，数据过期时需要优先删除
	time_t expiration_date;
	// 紧急数据(0-100)
	uint32_t urgent_data;
	// 原始长度
	uint64_t real_len;
	// 压缩方式
	uint8_t compress;
	// 数据
	data_buf_t payload;
} msg_t;

typedef struct {
	// 话题
	std::string topic;
	// 接收者(谁应答的消息)，可匿名
	std::string receiver;
	// 应答数据(不建议存放太多数据)
	data_buf_t payload;
	// ack时间
	msg_time_t send_time;
	// 实际收到ack时间
	msg_time_t recv_time;
} ack_msg_t;

// 推送消息
typedef struct {
	// 话题
	std::string topic;
	// QOS
	MsgQos qos;
	// 发送者，可匿名
	std::string sender;
	// 接收者
	std::vector<std::string> receivers;
	// receivers不为空时有效
	MsgRecvRule recv_rule;
	// 消息荷载
	struct iovec *payload;
	uint32_t iov_cnt;
	// 紧急数据(0-100)
	uint32_t urgent_data;
	// 有效期,0-未指定,毫秒
	time_t expiration;
} pub_msg_t;

// 订阅消息
typedef struct {
	// 话题
	std::string topic;
	// QOS
	MsgQos qos;
	// 谁发送的消息
	std::string sender;
	// 荷载
	data_buf_t payload;
	// 推送时间
	msg_time_t send_time;
	// 实际收到时间
	msg_time_t recv_time;
} sub_msg_t;

} // namespace mq

} // namespace bluseio
