#pragma once
#include "zutils_sdk.h"
#include"zstring.h"
#include"encode/zhash.h"
/**********************************************************************
 *@brief  生成UUID相关信息
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZUUIDUtils
{
public:
	static void generateUUIDString(const char* str,ZString &uuid);

	static void generateUUIDINT(const char* str, int64_t & uuid);
};
ZUTILS_NS_END