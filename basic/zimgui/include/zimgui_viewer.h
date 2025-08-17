#pragma once
#include"zimgui.h"

// 前向声明 ImGui 的相关结构
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