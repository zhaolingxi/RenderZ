#pragma once
#include"zimgui.h"
#include <cstddef> // For size_t


ZIMGUI_NS_BEGIN
struct ViewerDebugState {
    // --- 输入 (由 ZQt3DViewer 提供) ---
    bool hasModel = false;
    size_t vertexCount = 0;
    size_t faceCount = 0;

    // --- 输出 (由面板修改，ZQt3DViewer 读取并应用) ---
    enum class ShadingMode { Solid, Wireframe, Points };
    ShadingMode shadingMode = ShadingMode::Solid;

    bool showNormals = false;
    bool showBoundingBox = false;

    // 使用C-style数组代替QVector3D，这对于ImGui来说更直接
    float lightDirection[3] = { 0.408f, 0.408f, -0.816f }; // 预先归一化的 (0.5, 0.5, -1)
    float ambientIntensity = 0.1f;
};

class ZIMGUI_API ZIMGUIViewportDebugPanel
{

public:
    explicit ZIMGUIViewportDebugPanel() {};
    void draw(const char* title, ViewerDebugState& state);
protected:


};

ZIMGUI_NS_END