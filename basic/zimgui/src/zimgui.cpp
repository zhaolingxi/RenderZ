#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "zimgui_api.h" // ���������Լ��� C API ͷ�ļ�

// ����ļ����������� C++ �� ImGui ��ʵ��ϸ��
// �ⲿ��������ȫ��������Щ

// ȫ��������
static ImGuiContext* g_ImGuiContext = nullptr;

extern "C" {

    ZIMGUI_API void ZImGui_Init()
    {
        if (g_ImGuiContext) return;

        // 1. ���� ImGui ������
        g_ImGuiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(g_ImGuiContext);

        // 2. ���� IO
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // ���ü��̿���
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // ���� Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // ���ö��ӿ�

        // 3. ������ʽ
        ImGui::StyleColorsDark();

    }

    // ��������һ��ר�����ڳ�ʼ��OpenGL��˵ĺ���
    ZIMGUI_API void ZImGui_Init_OpenGL()
    {
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    // ��������һ��ר�����ڹر�OpenGL��˵ĺ���
    ZIMGUI_API void ZImGui_Shutdown_OpenGL()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    // ���޸ġ�ZImGui_Shutdown, ֻ������������
    ZIMGUI_API void ZImGui_Shutdown()
    {
        if (!g_ImGuiContext) return;

        // ����Ҫ������˹رշ����ȥ
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

        // �����ǵĶ����ṹ����� ImGui ������״̬
        if (input) {
            io.MousePos = ImVec2(input->mouse_x, input->mouse_y);
            for (int i = 0; i < 3; ++i) {
                io.MouseDown[i] = input->mouse_down[i];
            }
            io.MouseWheel = input->mouse_wheel;
        }

        // ��ʼ�µ�һ֡
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    ZIMGUI_API void ZImGui_Render()
    {
        if (!g_ImGuiContext) return;

        // ���ɻ�������
        ImGui::Render();

        // ʹ�õ�ǰ�󶨵� OpenGL �����Ľ��л���
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // ��������� Viewports������º���Ⱦ����ƽ̨����
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // extern "C"