#pragma once
#include"zqtgui.h"
#include<QAbstractListModel>

ZQTGUI_NS_BEGIN

class ZQTGUI_API ZQtViewerModel : public QAbstractListModel
{
	explicit ZQtViewerModel(QObject* parent = nullptr);
	virtual ~ZQtViewerModel();
};

ZQTGUI_NS_END