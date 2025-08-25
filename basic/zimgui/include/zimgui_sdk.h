#pragma once

// 名字空间定义
#define ZIMGuiNS zimgui
#define ZIMGUI_NS_BEGIN namespace ZIMGuiNS {
#define ZIMGUI_NS_END }

// 【修改】简化 ZIMGUI_API 的定义
// 因为我们决定使用静态库，所以不再需要 dllexport/dllimport
#define ZIMGUI_API



#include "zutils.h"
#include "zcore.h"
#include "zrender.h"
#include "zmath.h"
