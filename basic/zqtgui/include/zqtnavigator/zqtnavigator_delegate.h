#pragma once

#include"zqtgui.h"
#include <QStyledItemDelegate>
ZQTGUI_NS_BEGIN
class ZQTGUI_API ZQtNavigatorDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ZQtNavigatorDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
ZQTGUI_NS_END