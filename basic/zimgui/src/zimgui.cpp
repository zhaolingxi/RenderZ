#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "zimgui_api.h" // 包含我们自己的 C API 头文件

// 这个文件包含了所有 C++ 和 ImGui 的实现细节
// 外部调用者完全看不到这些

// 全局上下文
static ImGuiContext* g_ImGuiContext = nullptr;

extern "C" {

    ZIMGUI_API void ZImGui_Init()
    {
        if (g_ImGuiContext) return;

        // 1. 创建 ImGui 上下文
        g_ImGuiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(g_ImGuiContext);

        // 2. 配置 IO
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控制
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 启用 Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // 启用多视口

        // 3. 设置样式
        ImGui::StyleColorsDark();

    }

    // 【新增】一个专门用于初始化OpenGL后端的函数
    ZIMGUI_API void ZImGui_Init_OpenGL()
    {
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    // 【新增】一个专门用于关闭OpenGL后端的函数
    ZIMGUI_API void ZImGui_Shutdown_OpenGL()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    // 【修改】ZImGui_Shutdown, 只销毁主上下文
    ZIMGUI_API void ZImGui_Shutdown()
    {
        if (!g_ImGuiContext) return;

        // 【重要】将后端关闭分离出去
        // ImGui_ImplOpenGL3_Shutdown();

        ImGui::DestroyContext(g_ImGuiContext);
        g_ImGuiContext = nullptr;
    }



    ZIMGUI_API void ZImGui_NewFrame(int window_width, int window_height, float delta_time, const ZImGui_InputState* input)
    {
        if (!g_ImGuiContext) return;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(window_width), static_cast<float>(window_height));
        io.DeltaTime = delta_time > 0.0f ? delta_time : (1.0f / 60.0f);

        // 从我们的独立结构体更新 ImGui 的输入状态
        if (input) {
            io.MousePos = ImVec2(input->mouse_x, input->mouse_y);
            for (int i = 0; i < 3; ++i) {
                io.MouseDown[i] = input->mouse_down[i];
            }
            io.MouseWheel = input->mouse_wheel;
        }

        // 开始新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    ZIMGUI_API void ZImGui_Render()
    {
        if (!g_ImGuiContext) return;

        // 生成绘制数据
        ImGui::Render();

        // 使用当前绑定的 OpenGL 上下文进行绘制
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 如果启用了 Viewports，则更新和渲染其他平台窗口
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // extern "C"