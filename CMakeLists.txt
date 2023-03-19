cmake_minimum_required(VERSION 3.6)

# 主版本
set(BLUSEIO_MAJOR_VERSION 1)
# 次版本
set(BLUSEIO_MINOR_VERSION 0)
# 补丁版本
set(BLUSEIO_PATCH_VERSION 0)
set(BLUSEIO_VERSION "${BLUSEIO_MAJOR_VERSION}.${BLUSEIO_MINOR_VERSION}.${BLUSEIO_PATCH_VERSION}")
# 发布版本日期，该日期为该版本的开始开发日期
set(BLUSEIO_BASE_VERSION 22020827)

# 软件阶段版本
# base:架构版本，无实际功能
# alpha:内部功能版本，bug很多
# beta:无严重错误版本
# rc:基本成熟的版本
# release:发布版本
if("${BLUSEIO_PERIOD_VERSION}" STREQUAL "base")
    set(BLUSEIO_PERIOD_VERSION "base")
elseif("${BLUSEIO_PERIOD_VERSION}" STREQUAL "alpha")
    set(BLUSEIO_PERIOD_VERSION "alpha")
elseif("${BLUSEIO_PERIOD_VERSION}" STREQUAL "rc")
    set(BLUSEIO_PERIOD_VERSION "rc")
elseif("${BLUSEIO_PERIOD_VERSION}" STREQUAL "release")
    set(BLUSEIO_PERIOD_VERSION "release")
else()
    set(BLUSEIO_PERIOD_VERSION "beta")
endif()

project(
	bluseio
	VERSION ${BLUSEIO_MAJOR_VERSION}.${BLUSEIO_MINOR_VERSION}.${BLUSEIO_PATCH_VERSION}
	LANGUAGES C CXX
)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")
set(CMAKE_SKIP_RPATH TRUE)

include(GNUInstallDirs)

set(CMAKE_CONFIG_INSTALL_FILE ${PROJECT_BINARY_DIR}/config.toinstall.cmake)
set(CMAKE_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
set(INC_DIR ${PROJECT_SOURCE_DIR}/include CACHE PATH "bluseio inc")
set(LIB_DIR ${PROJECT_SOURCE_DIR}/_lib CACHE PATH "bluseio lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

add_custom_target(
	LINK_HEADERS ALL
	COMMENT "link headers..."
)

INCLUDE(CMakeLists_Headers.txt)

# macro(makeLink src dest target)
# 	add_custom_command(
# 		TARGET ${target} PRE_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dest}
# 		DEPENDS ${dest}
# 	)
# endmacro()

# add_custom_command(
# 	TARGET LINK_HEADERS PRE_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E make_directory ${INC_DIR}/${PROJECT_NAME}
# )

macro(makeLink src dest dstpath)
	file(MAKE_DIRECTORY ${dstpath})
	file(COPY ${src} DESTINATION ${dest})
	# COMMAND ${CMAKE_COMMAND} -E make_directory ${dstpath}
	# COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dest}
endmacro()

file(REMOVE_RECURSE ${INC_DIR}/${PROJECT_NAME})

foreach(header_file ${INCLUDE_HEADERS})
	string(REPLACE "/" ";" arr ${header_file})
	list (LENGTH arr length)
	list(SUBLIST arr 1 -1 file_name)
	math(EXPR length "${length} - 2")
	list(SUBLIST arr 1 ${length} file_path)
	list(JOIN file_name / file_name)
	list(JOIN file_path / file_path)
	file(MAKE_DIRECTORY ${INC_DIR}/${PROJECT_NAME}/${file_path})
	file(COPY ${PROJECT_SOURCE_DIR}/${header_file} DESTINATION ${INC_DIR}/${PROJECT_NAME}/${file_path})
	# list(GET arr -1 file_name)
	# makeLink(${PROJECT_SOURCE_DIR}/${header_file} ${INC_DIR}/${PROJECT_NAME}/${file_name} LINK_HEADERS)
	# makeLink(${PROJECT_SOURCE_DIR}/${header_file} ${INC_DIR}/${PROJECT_NAME}/${file_name} ${INC_DIR}/${PROJECT_NAME}/${file_path})
endforeach()

# 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "VERSION=${VERSION}")
message(STATUS "PERIOD_VERSION=${PERIOD_VERSION}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -O0 -g")
    message(STATUS "debug version")
	set(BLUSEIO_EDITION "debug")
    add_definitions(-DDEBUG)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -O3")
    message(STATUS "standard version")
	set(BLUSEIO_EDITION "standard")
    add_definitions(-DNDEBUG)
endif()

# 编译选项
set(PROJ_FLAGS " -pipe")
set(PROJ_FLAGS "${PROJ_FLAGS} -fPIC")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-unused-parameter")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-builtin-macro-redefined")
set(PROJ_FLAGS "${PROJ_FLAGS} -fno-omit-frame-pointer")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-implicit-fallthrough")
set(PROJ_FLAGS "${PROJ_FLAGS} -fstack-protector-all")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-deprecated-declarations")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-class-memaccess")
set(PROJ_FLAGS "${PROJ_FLAGS} -Wno-maybe-uninitialized")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJ_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROJ_FLAGS}")

# 编译宏
add_definitions(-D__const__=__unused__)

message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO is ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL is ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

# 执行系统命令，获取宏的结果
execute_process(COMMAND date +"%A %Y-%m-%d %H:%M:%S %z" TIMEOUT 5 OUTPUT_VARIABLE BLUSEIO_COMPILE_DATETIME OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND date +"%Y%m%d%H%M" TIMEOUT 5 OUTPUT_VARIABLE BLUSEIO_DATE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND /usr/bin/cat /proc/sys/kernel/random/uuid TIMEOUT 5 OUTPUT_VARIABLE BLUSEIO_UUID OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "compile time:BLUSEIO_COMPILE_DATETIME=${BLUSEIO_COMPILE_DATETIME}")
message(STATUS "compile date:BLUSEIO_DATE_VERSION=${BLUSEIO_DATE_VERSION}")
message(STATUS "compile uuid:BLUSEIO_UUID=${BLUSEIO_UUID}")

# 生成配置
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/bluseio/configure.h)

set(INCDIR include)
set(SRCDIR bluseio)
set(LIBDIR lib)
set(BINDIR bin)

# 头文件目录
set(INCDIRS . include bluseio)

include_directories(${INCDIRS} ${SRCDIR})

macro(list_source_directories srcs)
    unset(tmp)
    foreach(dir ${ARGN})
        aux_source_directory(${dir} tmp)
    endforeach()
    set(${srcs} ${tmp})
    list(FILTER ${srcs} EXCLUDE REGEX ".*_test\\.c")
endmacro()

macro(glob_headers_and_sources files)
    unset(tmp)
    foreach(dir ${ARGN})
        file(GLOB tmp ${dir}/*.h ${dir}/*.c ${dir}/*.hpp ${dir}/*.cpp ${dir}/*.cc)
        list(APPEND ${files} ${tmp})
    endforeach()
    list(FILTER ${files} EXCLUDE REGEX ".*_test\\.c")
endmacro()

#重新定义当前目标的源文件的__FILE__宏
function(redefine_file_macro targetname)
    #获取当前目标的所有源文件
    get_target_property(source_files "${targetname}" SOURCES)
    #遍历源文件
    foreach(sourcefile ${source_files})
        #获取当前源文件的编译参数
        get_property(defs SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS)
        #获取当前文件的绝对路径
        get_filename_component(filepath "${sourcefile}" ABSOLUTE)
        #将绝对路径中的项目路径替换成空,得到源文件相对于项目路径的相对路径
        string(REPLACE ${PROJECT_SOURCE_DIR}/${SRCDIR} "" relpath ${filepath})
        #将我们要加的编译参数(__FILE__定义)添加到原来的编译参数里面
        list(APPEND defs "__FILE__=\"${relpath}\"")
        #重新设置源文件的编译参数
        set_property(
            SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS ${defs}
            )
    endforeach()
endfunction()

# 库源码目录，越底层的放在越后
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/module)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/tools)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/utility)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/utility/thd)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/ecode)
set(PROJ_SRCDIRS ${PROJ_SRCDIRS} bluseio/base)

# 查找源码
list_source_directories(PROJ_SRCS ${PROJ_SRCDIRS})

# 静态库名
set(STATIC_LIB_NAME ${PROJECT_NAME})
# 动态库名
set(SHARED_LIB_NAME ${PROJECT_NAME}-shared)

add_library(
	${STATIC_LIB_NAME} STATIC
	${PROJ_SRCS}
)

add_library(
	${SHARED_LIB_NAME} SHARED
	${PROJ_SRCS}
)

# 设置动态库的输出名称
set_target_properties(${SHARED_LIB_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
# 设置动态库版本
set_target_properties(${SHARED_LIB_NAME} PROPERTIES VERSION ${BLUSEIO_VERSION} SOVERSION ${BLUSEIO_MAJOR_VERSION})

# 编译目标
# add_executable(${TARGET} ${BIOT_SRCS})

# 链接库
# target_link_libraries(${TARGET} ${BIOT_LIBS})

# 替换__FILE__为相对路径
redefine_file_macro(${PROJECT_NAME})
redefine_file_macro(${PROJECT_NAME}-shared)

####CONFIG

include(CMakePackageConfigHelpers)
set(CONFIG_INC_DIR ${INC_DIR})
set(CONFIG_LIB_DIR ${LIB_DIR})
configure_package_config_file(
	config.cmake.in
	${PROJECT_SOURCE_DIR}/config.cmake
	INSTALL_DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
	PATH_VARS CONFIG_INC_DIR CONFIG_LIB_DIR
)

set(CONFIG_INC_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(CONFIG_LIB_DIR ${CMAKE_INSTALL_LIBDIR})
configure_package_config_file(
	config.cmake.in
	${CMAKE_CONFIG_INSTALL_FILE}
	INSTALL_DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
	PATH_VARS CONFIG_INC_DIR CONFIG_LIB_DIR
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/bluseioConfigVersion.cmake
	VERSION ${BLUSEIO_VERSION}
	COMPATIBILITY AnyNewerVersion 
)

install(
	FILES ${CMAKE_CONFIG_INSTALL_FILE}
	DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
	COMPONENT devel
	RENAME bluseioConfig.cmake
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/bluseioConfigVersion.cmake
	DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
	COMPONENT devel
)

install(
	# FILES ${INCLUDE_HEADERS}
	DIRECTORY ${INC_DIR}/${PROJECT_NAME}
	# DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
	COMPONENT devel
)

install(
	FILES README.md
	DESTINATION "${CMAKE_INSTALL_DOCDIR}-${PROJECT_VERSION}"
	COMPONENT devel
)

install(
	TARGETS ${STATIC_LIB_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	COMPONENT devel
)

install(
	TARGETS ${SHARED_LIB_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	COMPONENT devel
)
