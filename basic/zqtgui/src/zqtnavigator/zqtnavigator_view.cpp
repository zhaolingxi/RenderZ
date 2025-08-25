#include "zqtnavigator/zqtnavigator_view.h"
#include"zqtnavigator/zqtnavigator_model.h"
#include"zqtnavigator/zqtnavigator_delegate.h"
#include <QMouseEvent>

ZQTGUI_NS_BEGIN


ZQtNavigatorView::ZQtNavigatorView(QWidget* parent) : QTreeView(parent)
{
    m_delegate = new ZQtNavigatorDelegate(this);
    setItemDelegate(m_delegate);

    // ���������٣��Ա������δ����ʱҲ�ܽ���mouseMoveEvent
    setMouseTracking(true);

    // ������������
    setHeaderHidden(true); // ������ͨ������Ҫ��ͷ
    setIndentation(15);    // ������������
    setAnimated(true);     // ��������Ч��
}

void ZQtNavigatorView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    int hoveredButton = 0;

    if (index.isValid()) {
        // --- �����޸Ĳ��� ---
        // 1. ֱ�ӻ�ȡitem�ľ�������
        QRect itemRect = visualRect(index);

        // 2. ʹ��ί�еĹ�������������ײ���
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

    // ֪ͨί�и�����ͣ��Ϣ
    m_delegate->setHoveredInfo(index, hoveredButton);

    // �����ػ棬�Ը�����ͣЧ��
    viewport()->update();

    QTreeView::mouseMoveEvent(event);
}

void ZQtNavigatorView::leaveEvent(QEvent* event)
{
    // ����뿪��ͼʱ�������ͣ��Ϣ
    m_delegate->setHoveredInfo(QModelIndex(), 0);
    viewport()->update();
    QTreeView::leaveEvent(event);
}

ZQTGUI_NS_END


