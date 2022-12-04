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
 * @file cache.h
 * @brief 
 * @author Jerry.Yu (jerry.yu512outlook.com)
 * @version 1.0.0
 * @date 2022-10-30
 * 
 * @copyright MIT License
 * 
 */
#pragma once
#include "msg.h"
#include <map>
#include <list>
#include "bluseio/utility/list.h"

namespace bluseio {
	
namespace mq {
	
typedef struct list_head msg_list_t;

// 1. 链式原始消息(用于缓存满淘汰)，节点保存了消息、信息，及其在topic链表中的地址
// 2. 链式原始消息，节点按过期时间(保存生存期的截至绝对时间)排序，最快过期的消息放在前边，最新的无有效期的消息放在末尾，记录有有效期和无有效期的分界节点
// 用于快速插入新的无有效期的消息
// 3. 每次的消息过期不需要遍历所有的消息是否过期，仅追溯到最后一个过期的消息，则下一个要过期的消息在头部

// 4. 每个topic维护一个链表，记录其缓存的消息，也是按照的时间序列(同原始消息链表)，并且保存了其在原始链表上的地址


} // namespace mq

} // namespace bluseio
