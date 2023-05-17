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
 * @file global_var.cpp
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-04-16
 * 
 * @copyright MIT License
 * 
 */
#include "global_var.h"
#include "log.h"
#include <map>
#include <iostream>

namespace bluseio {
namespace utility {

struct GVar {
    GVar(GVAR_TYPE type, const char* name, const char* alias, const char* value, const char* help, 
         const char* file, int line, void* addr)
		 : type(type), inner(false), name(name), alias(alias), value(value), help(help), 
		 file(file), lv(5), line(line), addr(addr) {
		if (help[0] == '>' && help[1] == '>') { /* defined inner */
			this->inner = true;
			this->help += 2;
		}

		// get level(0-9) at the beginning of help
		const char* const h = this->help;
		if (h[0] == '#' && '0' <= h[1] && h[1] <= '9' && (h[2] == ' ' || h[2] == '\0')) {
			lv = h[1] - '0';
			this->help += 2 + !!h[2];
		}
	}

    void set_value(const std::string& v) {
		switch (type)
		{
		case GVAR_BOOL:
			*static_cast<bool*>(addr) = "true" == v ? true : false;
			break;
		case GVAR_INT8:
			*static_cast<int8_t*>(addr) = static_cast<int8_t>(atoi(v.c_str()));
			break;
		case GVAR_UINT8:
			*static_cast<uint8_t*>(addr) = static_cast<uint8_t>(atoi(v.c_str()));
			break;
		case GVAR_INT16:
			*static_cast<int16_t*>(addr) = static_cast<int16_t>(atoi(v.c_str()));
			break;
		case GVAR_UINT16:
			*static_cast<uint16_t*>(addr) = static_cast<uint16_t>(atoi(v.c_str()));
			break;
		case GVAR_INT32:
			*static_cast<int32_t*>(addr) = static_cast<int32_t>(atoi(v.c_str()));
			break;
		case GVAR_UINT32:
			*static_cast<uint32_t*>(addr) = static_cast<uint32_t>(atoi(v.c_str()));
			break;
		case GVAR_INT64:
			*static_cast<int64_t*>(addr) = static_cast<int64_t>(atoll(v.c_str()));
			break;
		case GVAR_UINT64:
			*static_cast<uint64_t*>(addr) = static_cast<uint64_t>(atoll(v.c_str()));
			break;
		case GVAR_FLOAT:
			*static_cast<float*>(addr) = static_cast<float>(atof(v.c_str()));
			break;
		case GVAR_DOUBLE:
			*static_cast<double*>(addr) = static_cast<double>(atof(v.c_str()));
			break;
		case GVAR_STRING:
			*static_cast<std::string*>(addr) = v;
			break;
		
		default:
			blog_error("unknown type\n");
			break;
		}
	}

    std::string get_value() const {
		switch (type)
		{
		case GVAR_BOOL:
			return std::to_string(*static_cast<bool*>(addr));
			break;
		case GVAR_INT8:
			return std::to_string(*static_cast<int8_t*>(addr));
			break;
		case GVAR_UINT8:
			return std::to_string(*static_cast<uint8_t*>(addr));
			break;
		case GVAR_INT16:
			return std::to_string(*static_cast<int16_t*>(addr));
			break;
		case GVAR_UINT16:
			return std::to_string(*static_cast<uint16_t*>(addr));
			break;
		case GVAR_INT32:
			return std::to_string(*static_cast<int32_t*>(addr));
			break;
		case GVAR_UINT32:
			return std::to_string(*static_cast<uint32_t*>(addr));
			break;
		case GVAR_INT64:
			return std::to_string(*static_cast<int64_t*>(addr));
			break;
		case GVAR_UINT64:
			return std::to_string(*static_cast<uint64_t*>(addr));
			break;
		case GVAR_FLOAT:
			return std::to_string(*static_cast<float*>(addr));
			break;
		case GVAR_DOUBLE:
			return std::to_string(*static_cast<double*>(addr));
			break;
		case GVAR_STRING:
			return *static_cast<std::string*>(addr);
			break;
		
		default:
			blog_error("unknown type\n");
			break;
		}
	}
    void print() const {
		std::cout << CLR_GREEN << "    -" << this->name;
		if (*this->alias) std::cout << ", " << this->alias;
		std::cout.flush();
		std::cout << CLR_BLUE << "  " << this->help << '\n' << CLR_CLR
			<< "\ttype: " << this->type_str()
			<< "\t  default: " << this->value
			<< "\n\tfrom: " << this->file
			<< std::endl;
	}
    const char* type_str() const {
		switch (type) {
		case GVAR_BOOL: return "bool";
		case GVAR_INT8: return "int8_t";
		case GVAR_UINT8: return "uint8_t";
		case GVAR_INT16: return "int16_t";
		case GVAR_UINT16: return "uint16_t";
		case GVAR_INT32: return "int32_t";
		case GVAR_UINT32: return "uint32_t";
		case GVAR_INT64: return "int64_t";
		case GVAR_UINT64: return "uint64_t";
		case GVAR_FLOAT: return "float";
		case GVAR_DOUBLE: return "double";
		case GVAR_STRING: return "string";
		default:
			return "unknown type";
			break;
		}
	}

    GVAR_TYPE type;
    bool inner;
    const char* name;
    const char* alias;  // alias for this f lag
    const char* value;  // default value
    const char* help;   // help info
    const char* file;   // file where the flag is defined
    int line;           // line of the file where the flag is defined
    int lv;             // level: 0-9
    void* addr;         // point to the flag variable
};

std::map<std::string, GVar*> gVars() {
	static auto vars = new std::map<std::string, GVar*>();
}

}
}