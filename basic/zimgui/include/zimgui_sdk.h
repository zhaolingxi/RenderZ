#pragma once

// 名字空间定义
#define ZIMGuiNS zimgui
#define ZIMGUI_NS_BEGIN namespace ZIMGuiNS {
#define ZIMGUI_NS_END }

#ifdef _ZIMGUI_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define ZIMGUI_API __declspec(dllexport)
#else
// Linux平台导出接口
#define ZIMGUI_API
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define ZIMGUI_API __declspec(dllimport)
#else
// Linux平台导入接口
#define ZIMGUI_API 
#endif
#endif

#include "zutils.h"
#include "zcore.h"
#include "zrender.h"
#include "zmath.h"
