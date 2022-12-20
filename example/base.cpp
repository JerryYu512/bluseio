#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <bluseio/version.h>
#include <bluseio/base/singleton.h>
#include <bluseio/base/copyable.h>
#include <bluseio/base/noncopyable.h>
#include <bluseio/base/def.h>
#include <bluseio/base/any.h>
#include <bluseio/base/typedef.h>

class Apple: public bluseio::base::SingleTon<Apple> {
friend bluseio::base::SingleTon<Apple>;

public:
	void show() {
		std::cout << "Apple singleton:" << cnt_ << std::endl;
	}

	void inc() {
		cnt_++;
	}
private:
	int cnt_;
};

class Cp1: public bluseio::base::noncopyable {

};

class Cp2: public bluseio::base::copyable {

};

int main(void) {
	std::cout << bluseio::version() << std::endl;
	std::cout << bluseio::compile_time() << std::endl;
	// singleton
	Apple &app = bluseio::base::SingleTon<Apple>::GetInstance();
	app.show();
	app.inc();
	Apple &app2 = bluseio::base::SingleTon<Apple>::GetInstance();
	app2.show();

	// TODO:Any

	// noncopyable
	// Cp1 cp1;
	// Cp1 cp1_copy = cp1;

	// copyable
	Cp2 cp2;
	Cp2 cp2_copy = cp2;

	// bitmap
	// char bitmap[24] = {1};
	BITMAP_DEF(bitmap, 30, 1);
	BITMAP_DEF(bitmap1, 30, 0);
	std::cout << "bits: " << sizeof(bitmap)*8 << std::endl;

	std::cout << "bit 4:" << BITMAP_GET(bitmap, 4) << std::endl;
	std::cout << "bit2 4:" << BITMAP_GET(bitmap1, 4) << std::endl;
	BITMAP_CLR(bitmap, 24);
	std::cout << "bit 24:" << BITMAP_GET(bitmap, 24) << std::endl;
	BITMAP_SET(bitmap, 24);
	std::cout << "bit 24:" << BITMAP_GET(bitmap, 24) << std::endl;

	bluseio::base::MaxMin<int> range(30, 10, 11);

	std::cout << "20 in range: " << range.in_range(20) << std::endl;
	std::cout << "in range: " << range.overmax(20) << std::endl;
	std::cout << "in range: " << range.overmin(20) << std::endl;
	std::cout << "def val: " << range.value() << std::endl;

	char data_buf[1024];
	char data_buf2[1024];

	bluseio::base::DataBuf db(data_buf, sizeof(data_buf));
	bluseio::base::DataBuf db2(data_buf2, sizeof(data_buf2));

	db.append("hello bluseio", sizeof("hello bluseio"));

	std::cout << db.data() << std::endl;
	std::cout << "db ==: " << (db == db2) << std::endl;
	std::cout << "db equal: " << db.data_equal(db2) << std::endl;
	std::cout << "db valid: " << db.valid() << std::endl;
	std::cout << "db size: " << db.size() << std::endl;
	std::cout << "db free: " << db.free() << std::endl;
	db.clear();
	std::cout << "db size: " << db.size() << std::endl;
	std::cout << "db free: " << db.free() << std::endl;
	db.reset();
	std::cout << "db valid: " << db.valid() << std::endl;

	return 0;
}