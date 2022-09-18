cmake_minimum_required(VERSION 3.6)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")

project(biomq_test
		LANGUAGES C CXX
)

find_library(LIBRT rt)
find_package(OpenSSL REQUIRED)
# find_package(workflow REQUIRED CONFIG HINTS ..)
find_package(libhv REQUIRED CONFIG HINTS ..)
find_package(bluseio REQUIRED CONFIG HINTS ..)
include_directories(${OPENSSL_INCLUDE_DIR} ${LIBBLUSEIO_INCLUDE_DIR} ${BLUSEIO_INCLUDE_DIR})
link_directories(${LIBBLUSEIO_LIB_DIR})
link_directories(${BIOMQ_LIB_DIR})

find_program(CMAKE_MEMORYCHECK_COMMAND valgrind)
set(memcheck_command ${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1 --leak-check=full)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

enable_testing()
find_package(GTest REQUIRED)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++11 -fno-exceptions")

# 测试程序列表
set(TEST_LIST
)

set(BLUSEIO_LIB bluseio libhv pthread OpenSSL::SSL OpenSSL::Crypto ${LIBRT})

foreach(src ${TEST_LIST})
	add_executable(${src} EXCLUDE_FROM_ALL ${src}.cc)
	target_link_libraries(${src} ${BLUSEIO_LIB} GTest::GTest GTest::Main)
	add_test(${src} ${src})
	add_dependencies(check ${src})
endforeach()

foreach(src ${TEST_LIST})
	add_test(${src}-memory-check ${memcheck_command} ./${src})
endforeach()
