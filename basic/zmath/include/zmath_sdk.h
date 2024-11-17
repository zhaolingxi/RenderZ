#pragma once

// 名字空间定义
#define ZMathNS zmath
#define ZMATH_NS_BEGIN namespace ZMathNS {
#define ZMATH_NS_END }

#define HASGSL 1

#ifdef _ZMATH_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZMATH_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZMATH_API
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZMATH_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZMATH_API 
#endif
#endif
