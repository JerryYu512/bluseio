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
 * @file spinlock.h
 * @brief 
 * @author Jerry.Yu (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2023-03-18
 * 
 * @copyright MIT License
 * 
 */
#pragma once

#include <pthread.h>

namespace bluseio {
namespace utility {

class SpinLock {
public:
	SpinLock() : mtx_({0}) {}
	~SpinLock() {}

	void lock() {
		pthread_spin_lock(&mtx_);
	}

	bool trylock() {
		return 0 == pthread_spin_trylock(&mtx_);
	}

	void unlock() {
		pthread_spin_unlock(&mtx_);
	}

private:
	pthread_spinlock_t mtx_;
};

class SpinLockGuard {
public:
	SpinLockGuard(SpinLock &lck):lock(lck) {
		lock.lock();
	}

	~SpinLockGuard() {
		lock.unlock();
	}

private:
	SpinLock &lock;
};

}
}