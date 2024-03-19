#pragma once
#include "zqtgui_sdk.h"
#include <QObject>
#include <memory>
#include <unordered_map>
#include<QString>

namespace ZQTGuiNS {
class ZQTGUI_API QTPageManager : public QObject
{
public:


	/**
	  @brief 退出并销毁View
	  @param[in] viewlTopoOrder View在依赖树上的Topo序列,被依赖的View在后
	  @return 无
	 */
	void exitViewManager(const std::vector<QString> &viewlTopoOrder);

public:
	/**
	 @brief 加载界面样式
	 @note 只加载样式内容,不做样式切换
	 @note isUpdateResPath主要解决调试时需要加载多个resources路径下的qss文件的问题
	 @param[in] themeDir 样式文件夹路径, 示例: "./resources/theme"
	 @param[in] themeName 应用程序样式主题名称, 示例:"default"
	 @param[in] isUpdateResPath 是否需要替换qss文件中的资源路径
	 @return true/false
	 */
	bool loadTheme(const QString& themeDir,
		const QString& themeName,
		const bool isUpdateResPath = false);

	/**
 	  @brief 切换主题样式
 	  @param[in] themeName 样式主题名称
 	  @return true/false
 	 */
	bool changeTheme(const QString& themeName);

	/**
	 @brief 获取样式主题qss描述
	 @param[in] themeName 应用程序样式名称
	 @param[out] styleSheet 应用程序的样式qss描述
	 @return 样式描述
	 */
	bool getStyleSheet(const QString& themeName, QString& styleSheet);

public:
	/**
	 @brief 设置应用程序的语言
	 @param[in] langDir 语言包文件路径
	 @param[in] langName 语言名称
	 @return true/false
	 */
	bool loadLanguage(const QString& langDir, const QString& langName);

	/**
	  @brief 切换语言
	  @param[in] languageName 语言名称
	  @return true/false
	 */
	bool changeLanguage(const QString& languageName);


protected:
	QTPageManager(QObject* parent = nullptr);
	virtual ~QTPageManager();

protected:
	/**
	  @brief 初始化界面视图管理器
	  @return 无
	 */
	void initViewManager();


};
}
