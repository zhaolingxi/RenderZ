#include "zimgui_viewer_debug_panel.h"
#include <cmath> // For sqrtf
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

ZIMGUI_NS_BEGIN

// 【新增】一个本地的、不依赖任何框架的矢量归一化辅助函数
static void normalize_vec3(float v[3]) {
    float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 1e-5f) { // 避免除以零
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}


void ZIMGUIViewportDebugPanel::draw(const char* title, ViewerDebugState& state)
{
    // 开始一个新的ImGui窗口
    ImGui::Begin(title);

    // 1. 性能信息
    ImGui::Text(u8"性能 (Performance)");
    ImGui::Text(u8"平均 %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();

    // 2. 渲染模式
    ImGui::Text(u8"渲染模式 (Shading)");
    int currentMode = static_cast<int>(state.shadingMode);
    ImGui::RadioButton(u8"实体 (Solid)", &currentMode, 0); ImGui::SameLine();
    ImGui::RadioButton(u8"线框 (Wireframe)", &currentMode, 1); ImGui::SameLine();
    ImGui::RadioButton(u8"点云 (Points)", &currentMode, 2);
    state.shadingMode = static_cast<ViewerDebugState::ShadingMode>(currentMode);
    ImGui::Separator();

    // 3. 调试可视化
    ImGui::Text(u8"调试可视化 (Debug Viz)");
    ImGui::Checkbox(u8"显示法线", &state.showNormals);
    ImGui::Checkbox(u8"显示包围盒", &state.showBoundingBox);
    ImGui::Separator();

    // 4. 灯光
    ImGui::Text(u8"灯光 (Lighting)");
    // 【核心修改】直接将 state.lightDirection 传递给 ImGui::DragFloat3
    if (ImGui::DragFloat3(u8"光源方向", state.lightDirection, 0.01f, -1.0f, 1.0f)) {
        // 【核心修改】使用我们自己的辅助函数进行归一化
        normalize_vec3(state.lightDirection);
    }
    ImGui::SliderFloat(u8"环境光强度", &state.ambientIntensity, 0.0f, 1.0f);
    ImGui::Separator();

    // 5. 模型信息
    if (state.hasModel) {
        ImGui::Text(u8"模型信息 (Model Info)");
        ImGui::Text(u8"顶点数: %zu", state.vertexCount);
        ImGui::Text(u8"面数: %zu", state.faceCount);
    }
    else {
        ImGui::Text(u8"未加载模型 (No model loaded)");
    }

    ImGui::End();
}
ZIMGUI_NS_END