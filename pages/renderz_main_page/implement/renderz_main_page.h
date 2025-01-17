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

class RenderZMainPage : public QFrame
{
	Q_OBJECT
public:
	RenderZMainPage(QWidget* parent, const QString& viewName = "RenderZMainPage");
	virtual ~RenderZMainPage();

	bool createPage();
	bool createLayoutTop(QHBoxLayout* &pageViewLayoutTop);
	bool createLayoutMid(QHBoxLayout*& pageViewLayoutMid);
	bool createLayoutBtm(QHBoxLayout*& pageViewLayoutBtm);
	void initUI();
	void tempLoadTheme(QApplication* app=nullptr);


protected:
	std::shared_ptr<QString> readQssFiles(const QString& dirPath);

private:
	QGridLayout* mainLayout_{ nullptr }; /**< 页面主布局 */
	QWidget* mainSideBarLeft_{ nullptr }; /**< 侧边栏 */
	ZQtNavigatorView* leftNavigView_{nullptr};
	ZQtNavigatorModel* leftNavigModel_{nullptr};
	ZQtViewer* mianViewer_{ nullptr };   /*主viewer*/
	QWidget* mainSideBarRight_{ nullptr }; /**< 侧边栏 */
	QVBoxLayout* pageViewLayout_{ nullptr }; /**< 显示页面视图的布局(top|mid|btm) */



	QHBoxLayout* pageViewLayoutTop_{ nullptr };
	QHBoxLayout* pageViewLayoutMid_{ nullptr };
	QHBoxLayout* pageViewLayoutBtm_{ nullptr };

};
