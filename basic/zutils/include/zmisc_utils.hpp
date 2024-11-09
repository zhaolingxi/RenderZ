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
inline constexpr T z_align(const T & x, int c)
{
	return ((x)+(c)-1) & (~((c)-1));
}

template<typename T>
inline constexpr T z_align8(const T& x)
{
	return z_align<T>(x, 8);
}

// 取x, y的较大值
template<typename T>
inline constexpr T z_max(const T & value)
{
	return value;
}

template<typename T, typename... Args>
inline constexpr T z_max(const T & value, const Args&...rests)
{
	T maxVal = z_max(rests...);
	if (maxVal < value) {
		maxVal = value;
	}
	return maxVal;
}

// 取x, y的较小值
template<typename T>
inline constexpr T z_min(const T & value)
{
	return value;
}

template<typename T, typename... Args>
inline constexpr T z_min(const T & value, const Args&...rests)
{
	T minVal = z_min(rests...);
	if (minVal > value) {
		minVal = value;
	}
	return minVal;
}

// 数据右移
inline constexpr uint32_t  z_left_move(const uint32_t& val, const int bitCnt)
{
	return bitCnt <= 0 ? ((uint32_t)val) : (((uint32_t)val) << bitCnt);
}

// 数据强制转换 shared_ptr static cast
template<typename BaseType, typename T>
inline std::shared_ptr<T> z_sp_stcast(std::shared_ptr<BaseType> sp)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return std::static_pointer_cast<T>(sp);
}

// 数据强制转换 shared_ptr dynamic cast
template<typename BaseType, typename T>
inline std::shared_ptr<T> z_sp_dycast(std::shared_ptr<BaseType> sp)
{
	return std::dynamic_pointer_cast<T>(sp);
}

// 数据强制转换 shared_ptr const cast
template<typename T>
inline std::shared_ptr<T> z_sp_constcast(const std::shared_ptr<T> sp)
{
	return std::const_pointer_cast<T>(sp);
}

// 数据强制转换 pointer static cast
template<typename BaseType, typename T>
inline T* z_ptr_stcast(BaseType *ptr)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return static_cast<T*>(ptr);
}

// 数据强制转换 shared_ptr
template<typename BaseType, typename T>
inline T* z_ptr_dycast(BaseType *ptr)
{
	static_assert(std::is_base_of<BaseType, T>::value, "T must Derived BaseType");
	return dynamic_cast<T*>(ptr);
}

// 数据强制转换
template<typename T>
inline T& z_cast_to_ref(void* ptr)
{
	return *(reinterpret_cast<T*>(ptr));
}

// 指针强制转换
template<typename T>
inline T* z_cast_to_ptr(void* ptr)
{
	return reinterpret_cast<T*>(ptr);
}

template<typename T1, typename T2>
inline constexpr bool z_issame_ptr(const T1* p1, const T2 *p2)
{
	return reinterpret_cast<uint64_t>(p1) == reinterpret_cast<uint64_t>(p2);
}

// 获取类型名称
template<typename T>
inline constexpr std::string z_get_tpname()
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
inline constexpr std::string z_get_obj_tpname(const T *obj)
{
	std::string className = typeid(*obj).name();
	std::string::size_type pos = className.rfind(32);
	if (pos != std::string::npos) {
		className = className.substr(pos + 1);
	}
	return className;
}



// 检查字符串是否以特定字符结尾
inline bool z_stdstr_endwith(const std::string& str, char endCh) {
	return !str.empty() && str.back() == endCh;
}

// 检查字符串是否以特定子字符串结尾
inline bool z_stdstr_endwith(const std::string& str, const std::string& endStr) {
	if (endStr.length() > str.length()) {
		return false;
	}
	return str.rfind(endStr) == static_cast<size_t>(str.length() - endStr.length());
}

// 检查字符串是否以特定字符数组结尾
inline bool z_stdstr_endwith(const std::string& str, const char endStr[]) {
	std::string end(endStr);
	if (end.length() > str.length()) {
		return false;
	}
	return str.rfind(end) == static_cast<size_t>(str.length() - end.length());
}

