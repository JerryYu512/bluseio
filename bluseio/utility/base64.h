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
 * @file base64.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-25
 * 
 * @copyright MIT License
 * 
 */
#pragma once

namespace bluseio {
namespace utility {
	
enum {BASE64_OK = 0, BASE64_INVALID};

#define BASE64_ENCODE_OUT_SIZE(s)   (((s) + 2) / 3 * 4)
#define BASE64_DECODE_OUT_SIZE(s)   (((s)) / 4 * 3)

/// 编码
int base64_encode(const unsigned char *in, unsigned int inlen, char *out);

/// 解码
int base64_decode(const char *in, unsigned int inlen, unsigned char *out);

} // namespace utility
} // namespace bluseio
