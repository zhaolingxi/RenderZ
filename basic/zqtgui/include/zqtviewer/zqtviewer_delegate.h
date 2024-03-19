#pragma once
#include"zqtgui.h"
#include<QStyledItemDelegate>

ZQTGUI_NS_BEGIN
class ZQTGUI_API  ZQtViewerDelegate : public QStyledItemDelegate
{
public:
	explicit ZQtViewerDelegate(QObject* parent = nullptr);
	virtual ~ZQtViewerDelegate();
};

ZQTGUI_NS_END