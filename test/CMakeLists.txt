cmake_minimum_required(VERSION 3.6)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")

project(bio_test
		LANGUAGES C CXX
)

find_library(LIBRT rt)
find_package(OpenSSL REQUIRED)
# find_package(workflow REQUIRED CONFIG HINTS ..)
find_package(libhv REQUIRED CONFIG HINTS ..)
find_package(bluseio REQUIRED CONFIG HINTS ..)
include_directories(${OPENSSL_INCLUDE_DIR} ${LIBBLUSEIO_INCLUDE_DIR} ${BLUSEIO_INCLUDE_DIR} .. ../include)

# find_program(CMAKE_MEMORYCHECK_COMMAND valgrind)
# set(memcheck_command ${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1 --leak-check=full)

# add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# enable_testing()

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++14 -fno-exceptions")

# 测试程序列表
set(TEST_LIST
	test_case1
)

set(BLUSEIO_LIB bluseio hv pthread OpenSSL::SSL OpenSSL::Crypto ${LIBRT})

foreach(src ${TEST_LIST})
	string(REPLACE "-" ";" arr ${src})
	list(GET arr -1 bin_name)
	# add_executable(${src} EXCLUDE_FROM_ALL ${src}.cpp)
	add_executable(${bin_name} ${src}.cpp)
	target_link_libraries(${bin_name} ${BLUSEIO_LIB})
	# add_test(${src} ${src})
	# add_dependencies(check ${src})
endforeach()

# foreach(src ${TEST_LIST})
	# add_test(${src}-memory-check ${memcheck_command} ./${src})
# endforeach()
# 