#include "zqtnavigator/zqtnavigator_view.h"
#include"zqtnavigator/zqtnavigator_model.h"
#include"zqtnavigator/zqtnavigator_delegate.h"
#include <QMouseEvent>

ZQTGUI_NS_BEGIN


ZQtNavigatorView::ZQtNavigatorView(QWidget* parent) : QTreeView(parent)
{
    m_delegate = new ZQtNavigatorDelegate(this);
    setItemDelegate(m_delegate);

    // 启用鼠标跟踪，以便在鼠标未按下时也能接收mouseMoveEvent
    setMouseTracking(true);

    // 常用美化设置
    setHeaderHidden(true); // 导航树通常不需要表头
    setIndentation(15);    // 设置子项缩进
    setAnimated(true);     // 开启动画效果
}

void ZQtNavigatorView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    int hoveredButton = 0;

    if (index.isValid()) {
        // --- 核心修改部分 ---
        // 1. 直接获取item的矩形区域
        QRect itemRect = visualRect(index);

        // 2. 使用委托的公共函数进行碰撞检测
        if (m_delegate->getButtonRect(itemRect, 1).contains(event->pos())) {
            hoveredButton = 1;
        }
        else if (m_delegate->getButtonRect(itemRect, 2).contains(event->pos())) {
            hoveredButton = 2;
        }
        else if (m_delegate->getButtonRect(itemRect, 3).contains(event->pos())) {
            hoveredButton = 3;
        }
    }

    // 通知委托更新悬停信息
    m_delegate->setHoveredInfo(index, hoveredButton);

    // 请求重绘，以更新悬停效果
    viewport()->update();

    QTreeView::mouseMoveEvent(event);
}

void ZQtNavigatorView::leaveEvent(QEvent* event)
{
    // 鼠标离开视图时，清除悬停信息
    m_delegate->setHoveredInfo(QModelIndex(), 0);
    viewport()->update();
    QTreeView::leaveEvent(event);
}

ZQTGUI_NS_END


