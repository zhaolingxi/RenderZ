#pragma once

#include"zqtgui.h"
#include <QTreeView>

ZQTGUI_NS_BEGIN
#include <QTreeView>

class ZQtNavigatorDelegate;

class ZQTGUI_API ZQtNavigatorView : public QTreeView
{
    Q_OBJECT
public:
    explicit ZQtNavigatorView(QWidget* parent = nullptr);

signals:
    void button1Clicked(const QModelIndex& index);
    void button2Clicked(const QModelIndex& index);
    void button3Clicked(const QModelIndex& index);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    ZQtNavigatorDelegate* m_delegate;
};


ZQTGUI_NS_END