#pragma once
#ifdef _RENDERZ_MAIN_PAGE_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windows平台导出接口
#define RENDERZ_MAIN_PAGE_API __declspec(dllexport)
#else
// Linux平台导出接口
#define RENDERZ_MAIN_PAGE_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windows平台导入接口
#define RENDERZ_MAIN_PAGE_API __declspec(dllimport)
#else
// Linux平台导入接口
#define RENDERZ_MAIN_PAGE_API 
#endif
#endif
// 基础框架的头文件
#include "zutils.h"
#include "zcore.h"
#include "zqtgui.h"
