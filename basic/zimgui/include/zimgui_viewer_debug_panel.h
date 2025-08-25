#pragma once
#include"zimgui.h"
#include <cstddef> // For size_t


ZIMGUI_NS_BEGIN
struct ViewerDebugState {
    // --- ���� (�� ZQt3DViewer �ṩ) ---
    bool hasModel = false;
    size_t vertexCount = 0;
    size_t faceCount = 0;

    // --- ��� (������޸ģ�ZQt3DViewer ��ȡ��Ӧ��) ---
    enum class ShadingMode { Solid, Wireframe, Points };
    ShadingMode shadingMode = ShadingMode::Solid;

    bool showNormals = false;
    bool showBoundingBox = false;

    // ʹ��C-style�������QVector3D�������ImGui��˵��ֱ��
    float lightDirection[3] = { 0.408f, 0.408f, -0.816f }; // Ԥ�ȹ�һ���� (0.5, 0.5, -1)
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