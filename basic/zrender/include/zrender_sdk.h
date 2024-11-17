#pragma once

// 名字空间定义
#define ZRenderNS zrender
#define ZRENDER_NS_BEGIN  namespace ZRenderNS {
#define ZRENDER_NS_END  }

#ifdef _ZRENDER_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZRENDER_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZRENDER_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZRENDER_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZRENDER_API 
#endif
#endif
#include "zcore.h"
#include "zmath.h"

