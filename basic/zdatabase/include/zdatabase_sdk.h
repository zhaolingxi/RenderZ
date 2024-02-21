#pragma once

// 名字空间定义
#define ZDataBaseNS zdatabase
#define ZDATABASE_NS_BEGIN  namespace ZDataBaseNS {
#define ZDATABASE_NS_END  }

#ifdef _ZDATABASE_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZDATABASE_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZDATABASE_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZDATABASE_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZDATABASE_API 
#endif
#endif
#include "zutils.h"
#include "zcore.h"

