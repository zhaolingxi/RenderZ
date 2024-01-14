#pragma once

// 名字空间定义
#define ZCoreNS zcore
#define ZCORE_NS_BEGIN  namespace ZCoreNS {
#define ZCORE_NS_END  }

#ifdef _ZCORE_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZCORE_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZCORE_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZCORE_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZCORE_API 
#endif
#endif
#include "zutils.h"

