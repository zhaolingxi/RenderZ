#pragma once
#include "aagui.h"
#include <QVector>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <unordered_set>

class ToolBoxMainView : public UIViewBase
{
	Q_OBJECT
public:
	ToolBoxMainView(UIViewBase* parent, const QString& viewName = "ToolBoxMainView");
	virtual ~ToolBoxMainView();

	bool createView() override;

	void showEvent(QShowEvent* event) override;

protected:
	// 处理页面切换操作
	bool handleSwitchViewCmd(const int64_t cmdId, const std::any& param, QObject* sender);
	// 主页面的命令前置处理操作
	bool preHandleViewCmd(const int64_t cmdId, const std::any& param, QObject* sender);
private:
	GridLayout* mainLayout_{ nullptr }; /**< 页面主布局 */
	Widget* mainSideBar_{ nullptr }; /**< 侧边栏 */
	VBoxLayout* pageViewLayout_{ nullptr }; /**< 显示页面视图的布局 */
	// 当前打开的页面
	std::shared_ptr<UIViewBase> currentOpenedView_{ nullptr };
};
