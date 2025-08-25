#include "zimgui_viewer_debug_panel.h"
#include <cmath> // For sqrtf
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

ZIMGUI_NS_BEGIN

// ��������һ�����صġ��������κο�ܵ�ʸ����һ����������
static void normalize_vec3(float v[3]) {
    float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 1e-5f) { // ���������
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}


void ZIMGUIViewportDebugPanel::draw(const char* title, ViewerDebugState& state)
{
    // ��ʼһ���µ�ImGui����
    ImGui::Begin(title);

    // 1. ������Ϣ
    ImGui::Text(u8"���� (Performance)");
    ImGui::Text(u8"ƽ�� %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();

    // 2. ��Ⱦģʽ
    ImGui::Text(u8"��Ⱦģʽ (Shading)");
    int currentMode = static_cast<int>(state.shadingMode);
    ImGui::RadioButton(u8"ʵ�� (Solid)", &currentMode, 0); ImGui::SameLine();
    ImGui::RadioButton(u8"�߿� (Wireframe)", &currentMode, 1); ImGui::SameLine();
    ImGui::RadioButton(u8"���� (Points)", &currentMode, 2);
    state.shadingMode = static_cast<ViewerDebugState::ShadingMode>(currentMode);
    ImGui::Separator();

    // 3. ���Կ��ӻ�
    ImGui::Text(u8"���Կ��ӻ� (Debug Viz)");
    ImGui::Checkbox(u8"��ʾ����", &state.showNormals);
    ImGui::Checkbox(u8"��ʾ��Χ��", &state.showBoundingBox);
    ImGui::Separator();

    // 4. �ƹ�
    ImGui::Text(u8"�ƹ� (Lighting)");
    // �������޸ġ�ֱ�ӽ� state.lightDirection ���ݸ� ImGui::DragFloat3
    if (ImGui::DragFloat3(u8"��Դ����", state.lightDirection, 0.01f, -1.0f, 1.0f)) {
        // �������޸ġ�ʹ�������Լ��ĸ����������й�һ��
        normalize_vec3(state.lightDirection);
    }
    ImGui::SliderFloat(u8"������ǿ��", &state.ambientIntensity, 0.0f, 1.0f);
    ImGui::Separator();

    // 5. ģ����Ϣ
    if (state.hasModel) {
        ImGui::Text(u8"ģ����Ϣ (Model Info)");
        ImGui::Text(u8"������: %zu", state.vertexCount);
        ImGui::Text(u8"����: %zu", state.faceCount);
    }
    else {
        ImGui::Text(u8"δ����ģ�� (No model loaded)");
    }

    ImGui::End();
}
ZIMGUI_NS_END