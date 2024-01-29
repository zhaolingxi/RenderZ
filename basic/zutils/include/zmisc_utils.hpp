#pragma once
/**********************************************************************
 *@file  Zmisc_utils.hpp
 *@brief 通用宏定义和常用接口定义
 * 用于避免跨平台时，系统调用失效
 ***********************************************************************/

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <memory>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "zutils_define.h"


//////////////////////////////////////////////////////////////////////////////////////
//  常用操作宏定义
//////////////////////////////////////////////////////////////////////////////////////
#define ZSAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define ZSAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define ZSAFE_DELETE_VECTOR(v)	 do { for (size_t i =0; i < v.size(); i++) { ZSAFE_DELETE(v[i]) } v.clear(); } while(0)
#define ZSAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define ZRETURN_IF(cond)          do { if(cond) return; } while(0)
#define ZRETURN_TRUE_IF(cond)     do { if(cond) return true; } while(0)
#define ZRETURN_FALSE_IF(cond)    do { if(cond) return false; } while(0)
#define ZRETURN_NULL_IF(cond)     do { if(cond) return nullptr; } while(0)
#define ZBREAK_IF(cond)           if(cond) break;


// 声明类的对象不可复制
#define ZDISABLE_COPY(ClassName) \
	ClassName(ClassName&) = delete;\
	ClassName(ClassName&&) = delete;\
	void operator=(const ClassName&) = delete;\
	void operator=(const ClassName&&) = delete

// 取x, y的较大值
template<typename T>
inline constexpr T Z_align(const T & x, int c)
{
	return ((x)+(c)-1) & (~((c)-1));
}

template<typename T>
inline constexpr T Z_align8(const T& x)
{
	return Z_align<T>(x, 8);
}

// 取x, y的较大值
template<typename T>
inline constexpr T Zmax(const T & value)
{
	return value;
}

template<typename T, typename... Args>
inline constexpr T Zmax(const T & value, const Args&...rests)
{
	T maxVal = Zmax(rests...);
	if (maxVal < value) {
		maxVal = value;
	}
	return maxVal;
}

// 取x, y的较小值
template<typename T>
inline constexpr T Zmin(const T & value)
{
	return value;
}

template<typename T, typename... Args>
inline constexpr T Zmin(const T & value, const Args&...rests)
{
	T minVal = Zmin(rests...);
	if (minVal > value) {
		minVal = value;
	}
	return minVal;
}

// 数据右移
inline constexpr uint32_t  Zleft_move(const uint32_t& val, const int bitCnt)
{
	return bitCnt <= 0 ? ((uint32_t)val) : (((uint32_t)val) << bitCnt);
}

// 数据强制转换 shared_ptr static cast
template<typename BaseType, typename T>
inline std::shared_ptr<T> Zsp_stcast(std::shared_ptr<BaseType> sp)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return std::static_pointer_cast<T>(sp);
}

// 数据强制转换 shared_ptr dynamic cast
template<typename BaseType, typename T>
inline std::shared_ptr<T> Zsp_dycast(std::shared_ptr<BaseType> sp)
{
	return std::dynamic_pointer_cast<T>(sp);
}

// 数据强制转换 shared_ptr const cast
template<typename T>
inline std::shared_ptr<T> Zsp_constcast(const std::shared_ptr<T> sp)
{
	return std::const_pointer_cast<T>(sp);
}

// 数据强制转换 pointer static cast
template<typename BaseType, typename T>
inline T* Zptr_stcast(BaseType *ptr)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return static_cast<T*>(ptr);
}

// 数据强制转换 shared_ptr
template<typename BaseType, typename T>
inline T* Zptr_dycast(BaseType *ptr)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return dynamic_cast<T*>(ptr);
}

// 数据强制转换
template<typename T>
inline T& Zcast_to_ref(void* ptr)
{
	return *(reinterpret_cast<T*>(ptr));
}

// 指针强制转换
template<typename T>
inline T* Zcast_to_ptr(void* ptr)
{
	return reinterpret_cast<T*>(ptr);
}

template<typename T1, typename T2>
inline constexpr bool Z_issame_ptr(const T1* p1, const T2 *p2)
{
	return reinterpret_cast<uint64_t>(p1) == reinterpret_cast<uint64_t>(p2);
}

// 获取类型名称
template<typename T>
inline constexpr std::string Z_get_tpname()
{
	std::string className = typeid(T).name();
	std::string::size_type pos = className.rfind(32);
	if (pos != std::string::npos) {
		className = className.substr(pos + 1);
	}
	return className;
}

// 获取对象对应的类型名称
template<typename T>
inline constexpr std::string Z_get_obj_tpname(const T *obj)
{
	std::string className = typeid(*obj).name();
	std::string::size_type pos = className.rfind(32);
	if (pos != std::string::npos) {
		className = className.substr(pos + 1);
	}
	return className;
}

// 判断字符串是否以指定字符/字符串结尾
template<typename T1, typename T2>
inline bool Zstdstr_endwith(const T1& str, const T2 &endCh)
{
	static_assert(std::is_same<T1, std::string>::value, "T1 must be std::string or string_view");
	static_assert(std::is_same<T2, std::string>::value 
		|| std::is_same<T2, std::string_view>::value
		|| std::is_same<T2, char>::value, "T2 must be std::string, string_view or char ");
	return false;
}

template<>
inline bool Zstdstr_endwith(const std::string& str, const char &endCh)
{
	size_t len = str.length();
	if (len <= 0 ) {
		return false;
	}
	char lastCh = str[len - 1];
	return (lastCh == endCh);
}

template<>
inline bool Zstdstr_endwith(const std::string& str, const std::string &endStr)
{
	size_t len = str.length();
	size_t subLen = endStr.length();
	if (subLen <= 0 || len < subLen) {
		return false;
	}
	size_t pos = len - subLen;
	int ret = strcmp(str.c_str() + pos, endStr.c_str());
	return (ret == 0);
}


template<>
inline bool Zstdstr_endwith(const std::string& str, const std::string_view& endStr)
{
	size_t len = str.length();
	size_t subLen = endStr.length();
	if (subLen <= 0 || len < subLen) {
		return false;
	}
	size_t pos = len - subLen;
	int ret = strcmp(str.c_str() + pos, endStr.data());
	return (ret == 0);
}

// 匹配Zstdstr_endwith(str, "ab")
inline bool Zstdstr_endwith(const std::string& str, const char endStr[])
{
	size_t len = str.length();
	size_t subLen = strlen(endStr);
	if (subLen <= 0 || len < subLen) {
		return false;
	}
	size_t pos = len - subLen;
	int ret = strcmp(str.c_str() + pos, endStr);
	return (ret == 0);
}

// 检查是否到达最小Tick时间检查周期
// 通常对于大部分CPU而言,每1秒最多可执行10^8次非耗时操作
// 为减少获取系统时间的次数(减少系统调用)，因此可以采用如下
// 宏定义判断是否到达最小时间检查周期
// 按每1s最大循环次 10^8次计算，则每1ms循环次数为 10^5次
// 最少检查周期设为 5 ms,因此循环次数为: 500000
// 为提高程序运行效率可使用位运算判断整除
// a % b = a & (b-1)
#ifndef ZREACH_TICKTIME_CHECKCYCLE
#define ZREACH_TICKTIME_CHECKCYCLE(x)  (((x) & 49999) == 0)
#endif

// 日志输出宏定义
#if defined(WIN32) || defined(WIN64)
#define ZUTILS_LOG(typestr, format, ...)\
do { const char *fileName = __FILE__; int lineNum = __LINE__;\
	int pos = strlen(fileName) - 1;\
	for ( ; pos >= 0 && fileName[pos] != '/' && fileName[pos] != '\\'; pos--) { }\
	char buffer[1024] = {0}; snprintf(buffer, 1024, "[%s]:%s [line: %d]:", typestr,fileName + pos + 1, lineNum);\
	size_t len = strlen(buffer);\
	snprintf(buffer+len, 1024-len, format"\n",##__VA_ARGS__);\
	OutputDebugStringA(buffer);\
	fprintf(stderr, buffer);\
} while (0)
#else
#define ZUTILS_LOG(typestr, format, ...)\
do { const char *fileName = __FILE__; int lineNum = __LINE__;\
	int pos = strlen(fileName) - 1;\
	for ( ; pos >= 0 && fileName[pos] != '/' && fileName[pos] != '\\'; pos--) { }\
	char buffer[1024] = {0}; snprintf(buffer, 1024, "[%s]:%s [line: %d]:", typestr,fileName + pos + 1, lineNum);\
	size_t len = strlen(buffer);\
	snprintf(buffer+len, 1024-len, format"\n",##__VA_ARGS__);\
	fprintf(stderr, buffer);\
} while (0)
#endif

#define ZLOGFMTE(format, ...)   ZUTILS_LOG("ERROR", format,##__VA_ARGS__)
#define ZLOGFMTI(format, ...)   ZUTILS_LOG("INFO", format,##__VA_ARGS__)
#define ZLOGFMTW(format, ...)   ZUTILS_LOG("WARN", format,##__VA_ARGS__)
