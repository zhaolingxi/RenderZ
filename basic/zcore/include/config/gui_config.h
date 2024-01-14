#pragma once
#include "zcore_sdk.h"
#include "config/config_decorator.h"
/**********************************************************************
 *@file   widgetcfg_decorator.h
 *@date   2023/01/16 10 : 36
 *@author shanwenbin
 *@brief  界面插件框架配置
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API GUIConfig : public ConfigDecorator
{
public:
	GUIConfig(std::shared_ptr<ConfigBase> config);
	virtual ~GUIConfig();
public:
	// 网络服务插件所在文件夹
	std::shared_ptr<std::string> getViewPluginDir();
	// 网络服务插件名称
	ZSpVecStdString getViewPluginNames();
	// 网络服务插件路径
	ZSpVecStdString getViewPluginFilePaths();
};
ZCORE_NS_END
