#pragma once
#include "zcore_sdk.h"
#include "config/config_decorator.h"
#include "config/core_config.h"
#include "config/database_config.h"
#include "config/network_config.h"
#include "config/gui_config.h"
#include <memory>
#include <string>
#include <unordered_map>
/**********************************************************************
 *@brief  配置管理模块(逻辑模块主要配置)
***********************************************************************/
ZCORE_NS_BEGIN
class ConfigBase;
using CfgNameToConfigMap = std::unordered_map<std::string, std::shared_ptr<ConfigBase>>;
class ZCORE_API ConfigManager
{
	friend class AACoreSingleton;
public:
	/**
	  @brief 从配置文件夹中加载配置信息
	  @note 通过文件的后缀名确定配置类型
	  @param[in] cfgDir 配置文件夹
	  @return true/false
	 */
	bool loadConfigFromDir(const char* cfgDir);

public:
	/**
	  @brief 获取指定模块的配置信息(配置信息以模块作为单元进行管理)
	  @param[in] pluginName 插件模块名称
	  @return 配置信息
	 */
	std::shared_ptr<ConfigBase> getServiceConfigByName(const std::string& pluginName);
	std::shared_ptr<ConfigBase> getLogicConfigByName(const std::string& pluginName);
	std::shared_ptr<ConfigBase> getPageConfigByName(const std::string& pluginName);
	std::shared_ptr<ConfigBase> getBasicConfigByName(const std::string& moduleName);


	/**
	  @brief 创建配置对象的装饰器
	  @param[in] modName 模块名称
	  @return 配置对象
	 */
	template<typename T>
	std::shared_ptr<T> makeBasicCfgDecoratorByName(const std::string& moduleName)
	{
		static_assert(std::is_base_of<ConfigDecorator, T>::value, "T must Derived ConfigDecorator");
		auto cfg = getBasicConfigByName(moduleName);
		if (cfg == nullptr) {
			return nullptr;
		}
		auto decorator = std::make_shared<T>(cfg);
		decorator->parseConfig();
		return decorator;
	}

	template<typename T>
	std::shared_ptr<T> makeLogicCfgDecoratorByName(const std::string& pluginName)
	{
		static_assert(std::is_base_of<ConfigDecorator, T>::value, "T must Derived ConfigDecorator");
		auto cfg = getLogicConfigByName(pluginName);
		if (cfg == nullptr) {
			return nullptr;
		}
		auto decorator = std::make_shared<T>(cfg);
		decorator->parseConfig();
		return decorator;
	}

	template<typename T>
	std::shared_ptr<T> makeServiceCfgDecoratorByName(const std::string& pluginName)
	{
		static_assert(std::is_base_of<ConfigDecorator, T>::value, "T must Derived ConfigDecorator");
		auto cfg = getServiceConfigByName(pluginName);
		if (cfg == nullptr) {
			return nullptr;
		}
		auto decorator = std::make_shared<T>(cfg);
		decorator->parseConfig();
		return decorator;
	}

	template<typename T>
	std::shared_ptr<T> makePageCfgDecoratorByName(const std::string& pluginName)
	{
		static_assert(std::is_base_of<ConfigDecorator, T>::value, "T must Derived ConfigDecorator");
		auto cfg = getPageConfigByName(pluginName);
		if (cfg == nullptr) {
			return nullptr;
		}
		auto decorator = std::make_shared<T>(cfg);
		decorator->parseConfig();
		return decorator;
	}

public:
	// aacore
	// 业务插件目录和加载的插件名称
	std::shared_ptr<CoreConfig> getCoreConfig();

	// aagui
	// GUI窗体配置
	std::shared_ptr<GUIConfig> getGUIConfig();

	// aanetwork
	// 网络配置
	std::shared_ptr<NetworkConfig> getNetworkConfig();

protected:
	/**
 	  @brief 从文件中加载配置信息
 	  @param[in] filePath 配置文件路径
 	  @return true/false
 	 */
	bool loadConfigFromFile(const char* filePath);

	/**
	  @brief 从Json文件中加载配置信息
	  @param[in] filePath Json文件路径
	  @return true/false
	 */
	bool loadConfigFromJsonFile(const char* filePath);

	/**
	  @brief 从INI文件中加载配置信息
	  @param[in] filePath INI文件路径
	  @return true/false
	 */
	bool loadConfigFromINIFile(const char* filePath);

	/**
	  @brief 从json对象中加载配置信息
	  @param[in] jsonVal  json对象
	  @param[out] modNameToCfgMap 保存配置信息的map
	  @return 无
	 */
	void loadConfigFromJson(const Json::Value& cfgJson, CfgNameToConfigMap& modNameToCfgMap);

private:
	ConfigManager() = default;
	~ConfigManager() = default;
	ConfigManager(const ConfigManager& other) = delete;
	void operator=(const ConfigManager& other) = delete;
	ConfigManager(const ConfigManager&& other) = delete;
	void operator=(const ConfigManager&& other) = delete;
private:
	// 是否已初始化,防止接口重入
	bool isInited_{ false };
private:
	// 基础框架--aacore配置名称
	std::string coreCfgName_{ "aacore" };
	// 基础框架--aagui配置名称
	std::string guiCfgName_{ "aagui" };
	// 基础框架--网络基础库aanetwork配置项名称
	std::string networkCfgName_{ "aanetwork" };
	// 基础框架--配置信息列表
	CfgNameToConfigMap basicNameToConfigMap_;
private:
	// 业务模块配置项名称
	std::string logicsPluginCfgName_{ "logics_plugin_config" };
	// 单个业务模块插件的配置表
	CfgNameToConfigMap logicNameToConfigMap_;

	// 服务插件模块配置项名称
	std::string servicesPluginCfgName_{ "services_plugin_config" };
	// 单个服务插件模块配置
	CfgNameToConfigMap serviceNameToConfigMap_;

	// 页面插件模块配置项名称
	std::string pagesPluginCfgName_{ "pages_plugin_config" };
	// 单个页面插件模块配置
	CfgNameToConfigMap pageNameToConfigMap_;
};
ZCORE_NS_END
