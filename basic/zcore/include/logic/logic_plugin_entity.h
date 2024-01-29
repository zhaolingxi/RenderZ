#pragma once
#include "zcore_sdk.h"
#include "logic_plugin_module_info.h"
#include "config/config_manager.h"
#include "zlib_loader.h"

/**********************************************************************
 ***********************************************************************/
ZCORE_NS_BEGIN
class LogicModuleManager;
class ZCORE_API LogicPluginEntity
{
public:
	// 获取插件Module信息
	typedef void(*EntryGetPluginModuleInfo)(LogicPluginModuleInfo* pluginModuleInfo);
	// 安装插件Module
	typedef bool(*EntryInstallPluginModule)(const int64_t identity, LogicModuleManager* logicMgr, ConfigManager* configMgr);

public:
	LogicPluginEntity() = default;
	virtual ~LogicPluginEntity() = default;
public:
	const std::string& getName() const;
	const std::string& getFileDir() const;
	const std::string& getVersion() const;
	const std::string& getManufacture() const;

	/**
	 @brief 初始化插件实体
	 @param[in] pluginName 插件名称
	 @param[in] pluginDir  插件所在文件夹
	 @return true/false
	 */
	bool initEntity(const std::string& pluginName, const std::string& pluginDir);

	/**
	 @brief 退出插件实体
	 @return 无
	 */
	void exitEntity();

	/**
	 @brief 获取插件中包含的Module信息
	 @param[out] pluginModuleInfo 插件中包含的Module信息
	 @return true/false
	 */
	bool getModuleInfo(LogicPluginModuleInfo* pluginModuleInfo);

	/**
	 @brief 安装Module
	 @note 必须保证Module依赖的子Module已卸载完毕
	 @param[in] moduleId   Module的唯一标识
	 @param[in] logicMgr   业务模块管理器
	 @param[in] configMgr 配置管理器
	 @return true/false
	 */
	bool installModule(const int64_t moduleId, LogicModuleManager* logicMgr, ConfigManager* configMgr);

	/**
	 @brief 卸载Module
	 @param[in] ModuleId Module的唯一标识
	 @return 无
	 */
	void uninstallModule(const int64_t moduleId);

protected:
	/**
	 @brief 获取插件文件的路径
	 @param[in] pluginName 插件名称
	 @param[in] pluginDir  插件所在文件夹
	 @return 插件文件的路径
	 */
	std::shared_ptr<std::string> getPluginFilePath(const std::string& pluginName, const std::string& pluginDir);

private:
	// 插件对应的动态库实例
//	std::shared_ptr<ZLibLoader> pluginLibrary_{ nullptr };

private:
	std::string fileDir_{ "" }; // 插件文件所在文件夹
	std::string name_{ "" }; // 插件名称
	std::string version_{ "" }; // 插件版本
	std::string manufacture_{ "" }; // 生产商

private:
	// 获取插件Module信息入口函数
	EntryGetPluginModuleInfo getPluginModuleInfo_{ nullptr };
	// 安装插件Module入口函数
	EntryInstallPluginModule installPluginModule_{ nullptr };
};
ZCORE_NS_END
