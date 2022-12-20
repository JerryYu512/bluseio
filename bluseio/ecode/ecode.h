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
 * @file ecode.h
 * @brief 
 * @author  Jerry.Yu(astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-18
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include "ecode_def.h"

namespace bluseio {

class BioEcode {
public:
	static BioErrorCode ecode() {
		return code_;
	}
	static void ecode(BioErrorCode code) {
		code_ = code;
	}
	static const char* estring() {
		return query_error_code_en(code_);
	}
	static const char* estring(BioErrorCode code) {
		return query_error_code_en(code);
	}

private:
	static __thread BioErrorCode code_;
};

} // namespace bluseio
