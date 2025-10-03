#pragma once
#include <QVector>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <unordered_set>
#include <QGridLayout>
#include<QPushButton>
#include <QPainter>
#include <QStyleOption>
#include"zqtgui.h"
#include"renderz_main_page_sdk.h"
#include"zqtviewer/zqtviewer_view.h"
#include"zqtnavigator/zqtnavigator_view.h"
#include"zqtnavigator/zqtnavigator_model.h"
#include"zqtsidesetting/zqtsidesetting_view.h"
#include <QSplitter>

class RenderZMainPage : public QFrame
{
	Q_OBJECT
public:
	RenderZMainPage(QWidget* parent, const QString& viewName = "RenderZMainPage");
	virtual ~RenderZMainPage();

	bool createPage();
	bool createLayoutTop(QHBoxLayout* &pageViewLayoutTop);
	bool createLayoutMid(QHBoxLayout*& pageViewLayoutMid);
	QSplitter* createMidSplitter();
	bool createLayoutBtm(QHBoxLayout*& pageViewLayoutBtm);
	void initUI();
	void tempLoadTheme(QApplication* app=nullptr);
	void connectSlots();
public slots:
	void onSideViewBackgroundColorChanged(const QColor& newColor);
	void onLoadModel(const QModelIndex& slotindex, const QString& path);
	void onMenuClick(const QModelIndex& slotindex);
	void onMoreFunction(const QModelIndex& slotindex);
protected:
	void populateModelFromPath(ZQtNavigatorModel* model, const QString& path, const QModelIndex& parent);
private:
	QGridLayout* mainLayout_{ nullptr }; /**< ҳ�������� */
	QWidget* mainSideBarLeft_{ nullptr }; /**< ����� */
	ZQtNavigatorView* leftNavigView_{nullptr};
	ZQtNavigatorModel* leftNavigModel_{nullptr};
	ZQtViewer* mianViewer_{ nullptr };   /*��viewer*/
	SideSettingView* mainSideBarRight_{ nullptr }; /**< ����� */
	QVBoxLayout* pageViewLayout_{ nullptr }; /**< ��ʾҳ����ͼ�Ĳ���(top|mid|btm) */

	QHBoxLayout* pageViewLayoutTop_{ nullptr };
	QHBoxLayout* pageViewLayoutMid_{ nullptr };
	QHBoxLayout* pageViewLayoutBtm_{ nullptr };

};
