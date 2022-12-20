#ifndef BLUSEIO_UUID_H_
#define BLUSEIO_UUID_H_

#include <string.h>
#include <string>

namespace bluseio {
namespace utility {

void uuid_generate(char s[37]);

__attribute_used__ static char *uuid(void) {
	static __thread char s[38] = "";
	::memset(s, 0, sizeof(s));
	uuid_generate(s);
	return s;
}

__attribute_used__ static std::string uuid_str(void) {
	return uuid();
}

}
}
#endif // BLUSEIO_UUID_H_