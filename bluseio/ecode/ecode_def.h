#pragma once
#include <stdint.h>
#include <stddef.h>

namespace bluseio {

// 基础领域错误码为64位/8字节，0xffffffff
// 1-2字节为每个模块的具体错误码
// 3-4字节为子模块编号
// 5字节为主模块编号
// 6字节为错误标识
// 7-8字节为软件编号
// 0x0001 01 01 0001 0001

typedef uint64_t bio_err_t;

// 软件编号，需要每个软件自己定义
#ifndef ECODE_SOFTWARE_ID
#define ECODE_SOFTWARE_ID ((bio_err_t)(0x0000) << 62)
#endif

#define ECODE_BASE_ID_START(id) (ECODE_SOFTWARE_ID | ((bio_err_t)(id) << 32))
#define ECODE_BASE_ID_FROM_ECODE(code) (((bio_err_t)(code) >> 32) & 0xff)

/**
 * @brief 基础错误码ID
 * 
 */
typedef enum: bio_err_t {
	ECODE_BASE_NONE = 0,
	ECODE_BASE_SYSTEM,
	ECODE_BASE_SUB_SYSTEM,
	ECODE_BASE_THIRD,
	ECODE_BASE_ALGO,
	ECODE_BASE_DB,
	ECODE_BASE_HW,
	ECODE_BASE_DEVICE,
	ECODE_BASE_MEDIA,
	ECODE_BASE_NET,
	ECODE_BASE_STORAGE,
} EcodeBaseID;

/**
 * @brief 错误码主模块起始值
 * 
 */
typedef enum: bio_err_t {
	ECODE_OK 			= 0,
	// SYSTEM之前的为通用错误码
	ECODE_SYSTEM 		= ECODE_BASE_ID_START(ECODE_BASE_SYSTEM),		///< 系统错误码
	ECODE_SUB_SYSTEM 	= ECODE_BASE_ID_START(ECODE_BASE_SUB_SYSTEM),	///< 子系统错误码
	ECODE_THIED 		= ECODE_BASE_ID_START(ECODE_BASE_THIRD),		///< 依赖库错误码
	ECODE_ALGO 			= ECODE_BASE_ID_START(ECODE_BASE_ALGO),			///< 算法错误码
	ECODE_DB 			= ECODE_BASE_ID_START(ECODE_BASE_DB),			///< 数据库错误码
	ECODE_HW 			= ECODE_BASE_ID_START(ECODE_BASE_HW),			///< 硬件错误码
	ECODE_DEVICE 		= ECODE_BASE_ID_START(ECODE_BASE_DEVICE),		///< 设备错误码
	ECODE_MEDIA 		= ECODE_BASE_ID_START(ECODE_BASE_MEDIA),		///< 媒体错误码
	ECODE_NET 			= ECODE_BASE_ID_START(ECODE_BASE_NET),			///< 网络错误码
	ECODE_STORAGE 		= ECODE_BASE_ID_START(ECODE_BASE_STORAGE),		///< 存储错误码
	ECODE_UNKNOWN		= UINT64_MAX,
} EcodeDomainStart;

// 为错误码添加主模块值
#define ECODE_COMMON_GEN(code) (ECODE_COMMON | (code))
#define ECODE_SYSTEM_GEN(code) (ECODE_SYSTEM | (code))
#define ECODE_SUB_SYSTEM_GEN(code) (ECODE_SUB_SYSTEM | (code))
#define ECODE_THIRD_GEN(code) (ECODE_THIRD | (code))
#define ECODE_ALGO_GEN(code) (ECODE_ALGO | (code))
#define ECODE_DB_GEN(code) (ECODE_DB | (code))
#define ECODE_HW_GEN(code) (ECODE_HW | (code))
#define ECODE_DEVICE_GEN(code) (ECODE_DEVICE | (code))
#define ECODE_MEDIA_GEN(code) (ECODE_MEDIA | (code))
#define ECODE_NET_GEN(code) (ECODE_NET | (code))
#define ECODE_STORAGE_GEN(code) (ECODE_STORAGE | (code))

// 子模块

//////////////////////////////////////////////
// !系统子模块
/////////////////////////////////////////////
// 配置文件
#define ECODE_SYSTEM_CFGFILE(code) ECODE_SYSTEM_GEN(0x0001 | (code))
// 升级
#define ECODE_SYSTEM_UP(code) ECODE_SYSTEM_GEN(0x10001 | (code))

//////////////////////////////////////////////
// !网络子模块
/////////////////////////////////////////////
// http 1-600
#define ECODE_NET_HTTP(code) ECODE_NET_GEN(0x0001 | (code))
#define ECODE_NET_MQ(code) ECODE_NET_GEN(0x1001 | (code))

// 错误码，此处收集已经在使用的错误码，如果使用者需要自己单独定义错误码，则不使用下放的定义与接口
#define ECODE_MAP(XX) \
	XX(BIO_ECODE_OK = ECODE_OK, BIO_ECODE_OK, "success", "成功") \
	/* 通用错误码2-0xffff */ \
	/* 文件 */ \
	XX(BIO_COMMON_ECODE_FILE_NOT_EXIST = 2, BIO_COMMON_ECODE_FILE_NOT_EXIST, "file not exist", "文件不存在") \
	XX(BIO_COMMON_ECODE_FILE_OPEN_FAILED, BIO_COMMON_ECODE_FILE_OPEN_FAILED, "file wopen failed", "文件打开失败") \
	XX(BIO_COMMON_ECODE_FILE_WRITE_FAILED, BIO_COMMON_ECODE_FILE_WRITE_FAILED, "file write error", "文件写入异常") \
	XX(BIO_COMMON_ECODE_FILE_READ_FAILED, BIO_COMMON_ECODE_FILE_READ_FAILED, "file read error", "文件读取异常") \
	/* 内存 */ \
	XX(BIO_COMMON_ECODE_MEM_ALLOC_FAILED, BIO_COMMON_ECODE_MEM_ALLOC_FAILED, "memory alloc failed", "内存分配失败") \
	XX(BIO_COMMON_ECODE_MEM_INVALID, BIO_COMMON_ECODE_MEM_INVALID, "memory invalid", "内存不可用") \
	/* 数据 */ \
	XX(BIO_COMMON_ECODE_DATA_JSON_FMT_ERROR, BIO_COMMON_ECODE_DATA_JSON_FMT_ERROR, "json format error", "json格式错误") \
	XX(BIO_COMMON_ECODE_DATA_JSON_TO_STRUCT_ERROR, BIO_COMMON_ECODE_DATA_JSON_TO_STRUCT_ERROR, "json trans to struct failed", "json转结构体错误") \
	XX(BIO_COMMON_ECODE_DATA_STRUCT_TO_JSON_ERROR, BIO_COMMON_ECODE_DATA_STRUCT_TO_JSON_ERROR, "struct trans to json failed", "结构体转json错误") \
	XX(BIO_COMMON_ECODE_DATA_OVER_MAX, BIO_COMMON_ECODE_DATA_OVER_MAX, "data over max", "数据大于最大值") \
	XX(BIO_COMMON_ECODE_DATA_OVER_MIN, BIO_COMMON_ECODE_DATA_OVER_MIN, "data over min", "数据小于最小值") \
	XX(BIO_COMMON_ECODE_DATA_NOT_IN_RANGE, BIO_COMMON_ECODE_DATA_NOT_IN_RANGE, "data not in range", "数据不在范围内") \
	/* 流程 */ \
	/* 无效值 */ \
	XX(BIO_COMMON_ECODE_MAX = 0xffff, BIO_COMMON_ECODE_MAX, "invalid common code", "无效的通用错误码") \
	/* 系统错误码0x10000-0x1ffff */ \
	/* 配置文件 */ \
	XX(BIO_SYS_ECODE_CFG_FILE_NOT_EXIST = ECODE_SYSTEM_CFGFILE(0x0001), BIO_SYS_ECODE_CFG_FILE_NOT_EXIST, "configure file not exist", "配置文件不存在") \
	XX(BIO_SYS_ECODE_CFG_FILE_OPEN_FAILED, BIO_SYS_ECODE_CFG_FILE_OPEN_FAILED, "configure file open error", "配置文件打开错误") \
	XX(BIO_SYS_ECODE_CFG_FILE_HEADER_ERROR, BIO_SYS_ECODE_CFG_FILE_HEADER_ERROR, "configure file header error", "配置文件头错误") \
	XX(BIO_SYS_ECODE_CFG_FILE_HEADER_CHECK_FAILED, BIO_SYS_ECODE_CFG_FILE_HEADER_CHECK_FAILED, "configure file header check error", "配置文件头校验失败") \
	/* 升级 */ \
	XX(BIO_SYS_ECODE_UP_FILESIZE_MISMATCH = ECODE_SYSTEM_UP(0x0001), BIO_SYS_ECODE_UP_FILESIZE_MISMATCH, "Upgrade file size mismatch", "升级文件大小不匹配") \
	XX(BIO_SYS_ECODE_UP_FILESIZE_OVERSIZE, BIO_SYS_ECODE_UP_FILESIZE_OVERSIZE, "The upgrade file size is too large", "升级文件大小过大") \
	XX(BIO_SYS_ECODE_UP_DOMAIN_RESOLUTION_FAILED, BIO_SYS_ECODE_UP_DOMAIN_RESOLUTION_FAILED, "The download address of the upgrade file is abnormal", "升级文件下载地址异常") \
	XX(BIO_SYS_ECODE_UP_DOWNLOAD_TIMEOUT, BIO_SYS_ECODE_UP_DOWNLOAD_TIMEOUT, "Upgrade file download timed out", "升级文件下载超时") \
	XX(BIO_SYS_ECODE_UP_HASH_MISMATCH, BIO_SYS_ECODE_UP_HASH_MISMATCH, "Upgrade file digests do not match", "升级文件摘要不匹配") \
	XX(BIO_SYS_ECODE_UP_SIGNATURE_CHECK_FAILED, BIO_SYS_ECODE_UP_SIGNATURE_CHECK_FAILED, "Upgrade file signature verification failed", "升级文件验签失败") \
	XX(BIO_SYS_ECODE_UP_PACKAGE_ERROR, BIO_SYS_ECODE_UP_PACKAGE_ERROR, "The upgrade package is corrupted", "升级文件包损坏") \
	XX(BIO_SYS_ECODE_UP_PACKAGE_MEMORY_NOT_ENOUGH, BIO_SYS_ECODE_UP_PACKAGE_MEMORY_NOT_ENOUGH, "Insufficient memory for upgrade", "升级内存不足") \
	XX(BIO_SYS_ECODE_UP_WRITE_FAILED, BIO_SYS_ECODE_UP_WRITE_FAILED, "Burning failed", "烧录失败") \
	XX(BIO_SYS_ECODE_UP_USER_STOP, BIO_SYS_ECODE_UP_USER_STOP, "User stops upgrading", "用户停止升级") \
	XX(BIO_SYS_ECODE_UP_DOING, BIO_SYS_ECODE_UP_DOING, "upgrading", "正在升级") \
	XX(BIO_SYS_ECODE_UP_UNKNOWN, BIO_SYS_ECODE_UP_UNKNOWN, "low-level interface error", "底层接口错误") \
	XX(BIO_SYS_ECODE_UP_SOFT_VERSION_NOT_PERMISSION, BIO_SYS_ECODE_UP_SOFT_VERSION_NOT_PERMISSION, "Version not allowed to upgrade", "版本不允许升级") \
	XX(BIO_SYS_ECODE_UP_PLATFORM_MISMATCH, BIO_SYS_ECODE_UP_PLATFORM_MISMATCH, "Platform mismatch", "平台不匹配") \
	XX(BIO_SYS_ECODE_UP_DEVICE_MISMATCH, BIO_SYS_ECODE_UP_DEVICE_MISMATCH, "Device type mismatch", "设备类型不匹配") \
	XX(BIO_SYS_ECODE_UP_LANGUAGE_MISMATCH, BIO_SYS_ECODE_UP_LANGUAGE_MISMATCH, "language mismatch", "语言不匹配") \
	XX(BIO_SYS_ECODE_UP_VERSION_NOT_SUPPORT, BIO_SYS_ECODE_UP_VERSION_NOT_SUPPORT, "Upgrade protocol is not compatible", "升级协议不兼容") \
	/* 子系统错误码0x20000-0x2ffff */ \
	/* 第三方库错误码0x30000-0x3ffff */ \
	/* 算法错误码0x30000-0x3ffff */ \
	/* 数据库错误码0x30000-0x3ffff */ \
	/* 硬件错误码0x30000-0x3ffff */ \
	/* 设备错误码0x30000-0x3ffff */ \
	/* 媒体错误码0x30000-0x3ffff */ \
	/* 网络错误码0x30000-0x3ffff */ \
	XX(BIO_NET_ECODE_MQ_NODE_OVERMAX = ECODE_NET_MQ(0x0001), BIO_NET_ECODE_MQ_NODE_OVERMAX, "Upgrade protocol is not compatible", "节点已满") \
	XX(BIO_NET_ECODE_MQ_NODE_EXISTED, BIO_NET_ECODE_MQ_NODE_EXISTED, "Upgrade protocol is not compatible", "节点已存在") \
	XX(BIO_NET_ECODE_MQ_NODE_LITSEN, BIO_NET_ECODE_MQ_NODE_LITSEN, "Upgrade protocol is not compatible", "节点监听") \
	XX(BIO_NET_ECODE_MQ_URL_INVALID, BIO_NET_ECODE_MQ_URL_INVALID, "Upgrade protocol is not compatible", "url非法") \
	/* 存储错误码0x30000-0x3ffff */ \

// 错误码定义
typedef enum BioErrorCodeE: bio_err_t {
#define BIO_ERROR_CODE_MAP_TO_ENUM_DEF(expr, value, str, desc) expr,
	ECODE_MAP(BIO_ERROR_CODE_MAP_TO_ENUM_DEF)
#undef BIO_ERROR_CODE_MAP_TO_ENUM_DEF
} BioErrorCode;

/**
 * @brief 查询错误码，中文
 * 
 * @param no 
 * @return const char* 
 */
const char* query_error_code_zh(BioErrorCode no);
static const char* query_error_code_zh(bio_err_t no) {
	return query_error_code_zh((BioErrorCode)(no));
}

/**
 * @brief 查询错误码，英文
 * 
 * @param no 
 * @return const char* 
 */
const char* query_error_code_en(BioErrorCode no);
const char* query_error_code_en(bio_err_t no) {
	return query_error_code_en((BioErrorCode)(no));
}

/**
 * @brief 展示错误码
 * 
 */
void show_error_code(void);

} // namespace bluseio
