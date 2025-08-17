#pragma once

// 名字空间定义
#define ZIMGuiNS zimgui
#define ZIMGUI_NS_BEGIN namespace ZIMGuiNS {
#define ZIMGUI_NS_END }

// 【修改】简化 ZIMGUI_API 的定义
// 因为我们决定使用静态库，所以不再需要 dllexport/dllimport
#define ZIMGUI_API

/*
// 【或者】保留原来的逻辑，但默认情况是空白
// 只有在明确定义了 ZIMGUI_SHARED_LIB 时才启用导入导出
#ifdef ZIMGUI_SHARED_LIB
    #ifdef _ZIMGUI_EXPORTS
        #if defined(WIN32) || defined(WIN64)
            #define ZIMGUI_API __declspec(dllexport)
        #else
            #define ZIMGUI_API
        #endif
    #else
        #if defined(WIN32) || defined(WIN64)
            #define ZIMGUI_API __declspec(dllimport)
        #else
            #define ZIMGUI_API
        #endif
    #endif
#else
    // 默认编译为静态库，API宏为空
    #define ZIMGUI_API
#endif
*/

#include "zutils.h"
#include "zcore.h"
#include "zrender.h"
#include "zmath.h"
