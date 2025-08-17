#pragma once
#include"zimgui_sdk.h"
#ifdef __cplusplus
extern "C" {
#endif
    // 用一个简单结构体来传递输入状态，完全不依赖 Qt
    typedef struct {
        float mouse_x;
        float mouse_y;
        bool mouse_down[5]; // 0=left, 1=right, 2=middle
        float mouse_wheel;

        // 你可以添加更多键盘/手柄状态
        // ...
    } ZImGui_InputState;

    // API 函数
    ZIMGUI_API void ZImGui_Init();
    ZIMGUI_API void ZImGui_Shutdown();
    ZIMGUI_API void ZImGui_NewFrame(int window_width, int window_height, float delta_time, const ZImGui_InputState* input);
    ZIMGUI_API void ZImGui_Render();
    ZIMGUI_API void ZImGui_Init_OpenGL();
    ZIMGUI_API void ZImGui_Shutdown_OpenGL();

#ifdef __cplusplus
}
#endif