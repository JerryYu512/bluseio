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
 * @file pubsub.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-11-12
 * 
 * @copyright MIT License
 * 
 */
// 每条消息都有发送者sender(可匿名)，可选接收者：1.只让接收者们接收，2.保证接收者们接收，3.至少有其中一个接收者接收，4.不要求
// 应答消息必须标明谁接收了该消息并进行了应答.
// 当要求应答时，可以获取到消息被谁(接收者)接收了
// 订阅时，需要表明身份，该身份不被推送消息认可时将收不到发布消息；可选发送者：1.无发送者时，订阅所有该topic的消息，2.有发送者时，只订阅这些发送者发布的消息
// 应答可默认应答（内部返回最简单的应答报文），也可以自定义应答（应定义应答可填充自定义部分和选择应答时机）

#include <string>
#include <time.h>
#include <vector>
#include <functional>
#include <bluseio/ecode/ecode_def.h>
#include "msg.h"

namespace bluseio {
	
namespace mq {

using pub_msg_ret_cb = std::function<void(bio_err_t)>;
using pub_msg_ack_cb = std::function<void(bio_err_t, const ack_msg_t& ack)>;
	
class PublishClient {
public:
	/**
	 * @brief Construct a new Publish Client object
	 * 例子：net://127.0.0.1:8667/biomq/sample
	 * ipc://tmp/biomq-sample
	 * local://tmp/biomq-sample
	 * 
	 * @param url 推送的服务地址
	 */
	PublishClient(const std::string& url);

	// 推送
	/**
	 * @brief 
	 * 
	 * @param msg 推送的消息
	 * @param tm_ms 推送超时时间，-1阻塞，单位毫秒
	 * @param ack 是否需要应答
	 * @param ack_msg 应答消息
	 * @param ack_tm_ms 应答消息超时时间，-1阻塞，单位毫秒
	 * @return bio_err_t 
	 */
	bio_err_t publish(const pub_msg_t& msg, time_t tm_ms, bool ack, ack_msg_t& ack_msg, time_t ack_tm_ms = -1);

	// 异步推送
	/**
	 * @brief 
	 * 
	 * @param msg 
	 * @param tm_ms 推送超时时间
	 * @param ret_cb 推送结果反馈接口
	 * @param ack 是否需要应答
	 * @param ack_cb 应答反馈接口
	 * @param ack_tm_ms 应答消息超时时间，-1阻塞，单位毫秒
	 * @return bio_err_t
	 */
	bio_err_t publish_async(const pub_msg_t& msg, time_t tm_ms, pub_msg_ret_cb ret_cb, bool ack, pub_msg_ack_cb ack_cb, time_t ack_tm_ms = -1);
};

class SubscribeClient {
public:
	/**
	 * @brief Construct a new Subscribe Client object
	 * 
	 * @param url 订阅路径
	 * @param name 订阅者名称
	 */
	SubscribeClient(const std::string& url, const std::string& name);

	/**
	 * @brief 订阅
	 * 
	 * @param topic 订阅的话题
	 * @param priority 订阅的优先级
	 * @param senders 接收这些发送者的数据，为空时不指定
	 * @param tm_ms 订阅超时时间
	 * @return int 
	 */
	bio_err_t subscrbe(const std::string& topic, uint32_t priority, const std::vector<std::string>& senders, time_t tm_ms);

	/**
	 * @brief 接收订阅消息
	 * 
	 * @param msg 消息
	 * @param tm_ms 接收超时时间
	 * @return int 
	 */
	bio_err_t recv_msg(sub_msg_t& msg, time_t tm_ms);

	// 取消订阅topic
	bio_err_t unsubscrbe(const std::string& topic);

public:
	/// @brief  查询是否有订阅
	/// @param url 
	/// @param topic 
	/// @return 
	static bool query_sub(const std::string& url, const std::string& topic);

	/// @brief 查询订阅了哪些url
	/// @param url 
	/// @param topics 
	static void query_sub(const std::string& url, std::vector<std::string>& topics);

	/// @brief 查询订阅路由
	/// @param router 
	static void query_sub_router(std::string& router);
};

} // namespace mq

} // namespace bluseio
