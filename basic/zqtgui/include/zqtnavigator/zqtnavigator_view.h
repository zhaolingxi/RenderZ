#pragma once

#include"zqtgui.h"
#include <QTreeView>

ZQTGUI_NS_BEGIN
class ZQtNavigatorModel;
class ZQTGUI_API ZQtNavigatorView : public QTreeView {
    Q_OBJECT
public:
    explicit ZQtNavigatorView(QWidget* parent = nullptr, ZQtNavigatorModel* model=nullptr);
    virtual~ZQtNavigatorView() = default;

};

ZQTGUI_NS_END