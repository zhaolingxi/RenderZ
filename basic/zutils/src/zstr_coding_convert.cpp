#include "..\include\zstr_coding_convert.h"
#include<string>
#include<vector>
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <locale>
#include <codecvt>
#endif
/**********************************************************************
 *@brief 需要注意的是，尽量不要使用wstring_convert，可能会存在内存泄漏的情况，具体的信息参见
 * https://github.com/microsoft/STL/issues/443
 ***********************************************************************/
ZUTILS_NS_BEGIN
bool ZStrCodingConvert::mbcsToUnicode(const char* mbcsStr, wchar_t*& unicodeStr)
{
#ifdef _WIN32
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	int strLen = -1;
	if (strLen < 0) {
		strLen = strlen(mbcsStr);
	}
	// 计算转换成unicode后的长度
	int unicodeLen = ::MultiByteToWideChar(codepage, 0, mbcsStr, strLen, NULL, 0);
	std::vector<wchar_t> unicodeBuffer;
	unicodeBuffer.resize((size_t)unicodeLen + 1);
	// 将ASNI字符串转换成unicode
	unicodeLen = ::MultiByteToWideChar(codepage, 0, mbcsStr, strLen, (LPWSTR)unicodeBuffer.data(), unicodeLen);
	if (unicodeLen == 0) {
		return false;
	}
	unicodeStr = unicodeBuffer.data();
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	unicodeStr = converter.from_bytes(str);
#endif
	return true;
}

bool ZStrCodingConvert::unicodeToMbcs(const wchar_t* unicodeStr, char*& mbcsStr)
{
#ifdef _WIN32
	int unicodeLen = -1;
	if (unicodeLen < 0) {
		unicodeLen = wcslen(unicodeStr);
	}
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	int mbcsLen = WideCharToMultiByte(codepage, 0, unicodeStr, unicodeLen, 0, 0, 0, 0);
	std::vector<char> strBuffer;
	strBuffer.resize((size_t)mbcsLen + 1);
	mbcsLen = WideCharToMultiByte(codepage, 0, unicodeStr, unicodeLen, strBuffer.data(), mbcsLen, 0, 0);
	if (mbcsLen == 0) {
		return false;
	}
	mbcsStr = strBuffer.data();
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	mbcsStr = converter.to_bytes(unicodeStr);
#endif
	return true;
}

bool ZStrCodingConvert::unicodeToUtf8(const wchar_t* unicodeStr, char*& utf8Str)
{
#ifdef _WIN32
	int unicodeLen = -1;
	if (unicodeLen < 0) {
		unicodeLen = wcslen(unicodeStr);
	}
	std::vector<char> strBuffer;
	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, unicodeLen, 0, 0, 0, 0);
	strBuffer.resize((size_t)utf8Length + 1);
	utf8Length = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, unicodeLen, (LPSTR)strBuffer.data(), utf8Length, 0, 0);
	if (utf8Length == 0) {
		return false;
	}
	utf8Str = strBuffer.data();
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	utf8Str = converter.to_bytes(unicodeStr);
#endif
	return true;
}

bool ZStrCodingConvert::utf8ToUnicode(const char* utf8Str, wchar_t*& unicodeStr)
{
#ifdef _WIN32
	int utf8Len = -1;
	if (utf8Len < 0) {
		utf8Len = strlen(utf8Str);
	}
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str, utf8Len, NULL, 0);
	std::vector<wchar_t> unicodeBuffer;
	unicodeBuffer.resize((size_t)unicodeLen + 1);
	unicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, unicodeBuffer.data(), unicodeLen);
	if (unicodeLen == 0) {
		return false;
	}
	unicodeStr = unicodeBuffer.data();
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	unicodeStr = converter.from_bytes(utf8Str);
#endif
	return true;
}

bool ZStrCodingConvert::utf8ToMbcs(const char* utf8Str, char*& mbcsStr)
{
	wchar_t* unicodeStr = nullptr;
	if (!utf8ToUnicode(utf8Str, unicodeStr) || !unicodeStr) {
		return false;
	}
	if (!unicodeToMbcs(unicodeStr, mbcsStr)) {
		return false;
	}
	return true;
}

bool ZStrCodingConvert::mbcsToUtf8(const char* mbcsStr, char*& utf8Str)
{
	wchar_t* unicodeStr = nullptr;
	if (!mbcsToUnicode(mbcsStr, unicodeStr) || !unicodeStr) {
		return false;
	}
	if (!unicodeToMbcs(unicodeStr, utf8Str)) {
		return false;
	}
	return true;
}
ZUTILS_NS_END