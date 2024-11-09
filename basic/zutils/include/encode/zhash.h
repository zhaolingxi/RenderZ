#pragma once
#include "zutils_sdk.h"
#include <cstdint>
#include <memory>
#include <string>
/**********************************************************************
 *@brief  HASH函数(封装自xxhash)
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZHash
{
protected:
	static constexpr uint32_t HASH_SEED = ((uint32_t)'Z' << 24) | ((uint32_t)'G' << 16) | ((uint32_t)'H' << 8) | (uint32_t)'Z';
public:
	/**
	 @brief 求指定字符串的hash值
	 @param[in] str 字符串
	 @param[in] len 字符串的长度
	 @return 字符串的hash值
	 */
	static uint32_t encodeStr32(const char* str, const int len = -1);
	//static uint64_t encodeStr64(const char *str, const int len = -1);

	/**
	 @brief  求指定数据的hash值
	 @param[in] dataBuffer 数据缓存
	 @param[in] dataSize   数据大小
	 @return 数据的hash值
	 */
	static uint32_t encode(const uint8_t *dataBuffer, const int dataSize);
};
ZUTILS_NS_END
