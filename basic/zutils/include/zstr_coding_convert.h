#pragma once
#include "zutils_sdk.h"

/**********************************************************************
 *@brief 字符编码转换, 多字节|unicode|utf8
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZStrCodingConvert
{
public:
	static bool mbcsToUnicode(const char* mbcsStr, wchar_t*& unicodeStr);

	static bool unicodeToMbcs(const wchar_t* unicodeStr, char*& mbcsStr);

	static bool unicodeToUtf8(const wchar_t* unicodeStr, char*& utf8Str);

	static bool utf8ToUnicode(const char* utf8Str, wchar_t*& unicodeStr);

	static bool utf8ToMbcs(const char* utf8Str, char*& mbcsStr);

	static bool mbcsToUtf8(const char* mbcsStr, char*& utf8Str);
};
ZUTILS_NS_END