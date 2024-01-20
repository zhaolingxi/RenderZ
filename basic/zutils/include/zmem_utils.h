#pragma once
#include "zutils_sdk.h"
/**********************************************************************
 *@brief  内存相关操作：拷贝、序列化、反序列化
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZMemUtils
{
public:
	static void memCopy(void *dst, const int dstSize, const void *src, const int srcSize);
};
ZUTILS_NS_END