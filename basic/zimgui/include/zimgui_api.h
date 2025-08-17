#pragma once
#include"zimgui_sdk.h"
#ifdef __cplusplus
extern "C" {
#endif
    // ��һ���򵥽ṹ������������״̬����ȫ������ Qt
    typedef struct {
        float mouse_x;
        float mouse_y;
        bool mouse_down[5]; // 0=left, 1=right, 2=middle
        float mouse_wheel;

        // �������Ӹ������/�ֱ�״̬
        // ...
    } ZImGui_InputState;

    // API ����
    ZIMGUI_API void ZImGui_Init();
    ZIMGUI_API void ZImGui_Shutdown();
    ZIMGUI_API void ZImGui_NewFrame(int window_width, int window_height, float delta_time, const ZImGui_InputState* input);
    ZIMGUI_API void ZImGui_Render();
    ZIMGUI_API void ZImGui_Init_OpenGL();
    ZIMGUI_API void ZImGui_Shutdown_OpenGL();

#ifdef __cplusplus
}
#endif