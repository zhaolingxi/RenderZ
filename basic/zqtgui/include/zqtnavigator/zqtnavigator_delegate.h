#pragma once

#include"zqtgui.h"
#include <QStyledItemDelegate>
ZQTGUI_NS_BEGIN

class ZQTGUI_API ZQtNavigatorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZQtNavigatorDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    // 处理按钮点击
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

    void setHoveredInfo(const QModelIndex& index, int button);

    QRect getButtonRect(const QRect& itemRect, int buttonNumber) const;


private:

    QIcon m_button1Icon;
    QIcon m_button2Icon;
    QIcon m_button3Icon;

    // 存储当前鼠标悬停的按钮信息
    QPersistentModelIndex m_hoveredIndex;
    int m_hoveredButton; // 0: none, 1: button1, 2: button2, 3: button3
};
ZQTGUI_NS_END