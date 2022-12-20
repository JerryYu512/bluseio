/**
 * MIT License
 *
 * Copyright © 2021 <wotsen>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the “Software”), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @file os.cpp
 * @brief
 * @author wotsen (astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2021-07-25
 *
 * @copyright MIT License
 *
 */
#include "os.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <hv/hatomic.h>

#ifdef __linux__
#include <errno.h>
#include <sys/sysinfo.h>
#endif

#ifdef __APPLE__
#include <mach/mach_host.h>
#include <sys/sysctl.h>
#endif

namespace bluseio {
namespace utility {

static struct utsname t_info;
static hatomic_flag_t flag = HATOMIC_FLAG_INIT;

// 默认系统call
static int (*system_fun)(const char *) = system;

void set_default_system_call(int (*fun)(const char *)) {
    if (fun) {
        system_fun = fun;
    }
}

int syscmd(const char *cmd) { return system_fun(cmd); }

// 获取平台架构
const char *get_arch(void) {
    if (!hatomic_flag_test_and_set(&flag)) {
        uname(&t_info);
    }
    return t_info.machine;
}

// 获取平台信息
const char *get_platform(void) {
    if (!hatomic_flag_test_and_set(&flag)) {
        uname(&t_info);
    }
    // TODO
    return NULL;
}

// 获取系统名称
const char *get_sysname(void) {
    if (!hatomic_flag_test_and_set(&flag)) {
        uname(&t_info);
    }
    return t_info.sysname;
}

// 获取处理器信息
const char *get_processor(void) {
    // TODO
    return "null";
}

// 获取系统版本
const char *get_version(void) {
    if (!hatomic_flag_test_and_set(&flag)) {
        uname(&t_info);
    }
    return t_info.version;
}

int get_ncpu(void) { return sysconf(_SC_NPROCESSORS_CONF); }

int get_meminfo(meminfo_t *mem) {
#if defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info)) {
        return errno;
    }
    mem->total = info.totalram * info.mem_unit >> 10;
    mem->free = info.freeram * info.mem_unit >> 10;

    return 0;
#elif defined(__APPLE__)
    uint64_t memsize = 0;
    size_t size = sizeof(memsize);
    int which[2] = {CTL_HW, HW_MEMSIZE};
    sysctl(which, 2, &memsize, &size, NULL, 0);
    mem->total = memsize >> 10;

    vm_statistics_data_t info;
    mach_msg_type_number_t count = sizeof(info) / sizeof(integer_t);
    host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&info, &count);
    mem->free = ((uint64_t)info.free_count * sysconf(_SC_PAGESIZE)) >> 10;

    return 0;
#else
    return -10;
#endif
}

char* env(const char* name, char* buf, size_t len) {
    char *x = getenv(name);
    snprintf(buf, len, "%s", x);
    return buf;
}

void daemon(void) {
    // int r = ::daemon(1, 0);
    // (void)r;
}

}
}
