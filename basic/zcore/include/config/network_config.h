#pragma once
#include "zcore_sdk.h"
#include "config/config_decorator.h"
/**********************************************************************
 *@brief  网络基础库配置
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API NetworkConfig : public ConfigDecorator
{
public:
	NetworkConfig(std::shared_ptr<ConfigBase> config);
	virtual ~NetworkConfig();
public:
	// 获取域控端http端口
	int getDevHttpPort();
	// 获取域控端websocket端口
	int getDevWsPort();
	
	// Web端Http静态文件根目录
	std::shared_ptr<std::string> getWebDocRoot();
	// Web端支持的静态文件后缀
	ZSpVecStdString getWebFileTypes();
	// Web端Http端口
	int getWebHttpPort();
	// Web端Websocket端口
	int getWebWsPort();
	// Web端http线程梳理
	int getWebHttpThreadCnt();
	
	// 网络服务插件所在文件夹
	std::shared_ptr<std::string> getServicePluginDir();
	// 网络服务插件名称
	ZSpVecStdString getServicePluginNames();
	// 网络服务插件路径
	ZSpVecStdString getServicePluginFilePaths();
};
ZUTILS_NS_END