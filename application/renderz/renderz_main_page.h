#pragma once
#include <QVector>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <unordered_set>
#include <QGridLayout>
#include<QPushButton>

class RenderZMainPage : public QWidget
{
	Q_OBJECT
public:
	RenderZMainPage(QWidget* parent, const QString& viewName = "RenderZMainPage");
	virtual ~RenderZMainPage();

	bool createPage();
	void initUI();


protected:
	void addPushBtm(QHBoxLayout*& ioLayout);

private:
	QGridLayout* mainLayout_{ nullptr }; /**< 页面主布局 */
	QWidget* mainSideBarLeft_{ nullptr }; /**< 侧边栏 */
	QWidget* mainSideBarRight_{ nullptr }; /**< 侧边栏 */
	QVBoxLayout* pageViewLayout_{ nullptr }; /**< 显示页面视图的布局(top|mid|btm) */
	QHBoxLayout* pageViewLayoutTop_{ nullptr };
	QHBoxLayout* pageViewLayoutMid_{ nullptr };
	QHBoxLayout* pageViewLayoutBtm_{ nullptr };

};
