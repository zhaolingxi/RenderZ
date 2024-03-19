#pragma once

// 名字空间定义
#define ZUtilsNS zutils
#define ZUTILS_NS_BEGIN  namespace ZUtilsNS {
#define ZUTILS_NS_END  }
#ifdef _ZUTILS_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZUTILS_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZUTILS_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZUTILS_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZUTILS_API 
#endif
#endif
