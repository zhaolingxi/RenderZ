#pragma once
#include"zimgui.h"

// ǰ������ ImGui ����ؽṹ
struct ImGuiContext;

ZIMGUI_NS_BEGIN

class ZIMGUI_API ZQtImGuiViewer 
{

public:
    explicit ZQtImGuiViewer();
    ~ZQtImGuiViewer() override;

protected:


private:

    ImGuiContext* imguiContext_{ nullptr };
};

ZIMGUI_NS_END