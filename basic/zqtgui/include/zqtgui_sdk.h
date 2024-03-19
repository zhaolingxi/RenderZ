#pragma once

// 名字空间定义
#define ZQTGuiNS zqtgui
#define ZQTGUI_NS_BEGIN namespace ZQTGuiNS {
#define ZQTGUI_NS_END }

#ifdef _ZQTGUI_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZQTGUI_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZQTGUI_API
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZQTGUI_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZQTGUI_API 
#endif
#endif
#include "zutils.h"
#include "zcore.h"
