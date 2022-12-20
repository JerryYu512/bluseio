#include <iostream>
#include <thread>
#include <bluseio/ecode/ecode.h>

void ecode_change(void) {
	bluseio::BioEcode::ecode(bluseio::BIO_COMMON_ECODE_FILE_READ_FAILED);
	std::cout << bluseio::BioEcode::ecode() << " - " << bluseio::BioEcode::estring() << std::endl;
}

int main(void) {
	// bluseio::show_error_code();
	std::cout << bluseio::BioEcode::ecode() << " - " << bluseio::BioEcode::estring() << std::endl;

	std::thread tid(ecode_change);
	tid.join();
	std::cout << bluseio::BioEcode::ecode() << " - " << bluseio::BioEcode::estring() << std::endl;

	return 0;
}