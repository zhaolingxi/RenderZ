#pragma once
#include "zcore_sdk.h"
#include <map>
#include <vector>
#include "logic_plugin_entity.h"
/**********************************************************************
 *@file  logic_plugin_manager.h
 *@date  2023/07/01 16 : 02
 *@author shanwenbin
 *@brief 业务模块插件管理器
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API LogicPluginManager
{
public:
	LogicPluginManager() = default;
	virtual ~LogicPluginManager() = default;
public:
	/**
	 @brief 设置插件目录
	 @param[in] pluginDir 插件目录
	 @return 无
	 */
	void setPluginDir(const char* pluginDir);

	/**
	 @brief 插件是否已加载
	 @param[in] pluginName 插件名称
	 @return true/false
	 */
	bool isPluginLoaded(const char* pluginName);

	/**
	 @brief 加载插件
	 @param[in] pluginDir  插件所在文件夹
	 @param[in] pluginName 插件名称,不能带.so/.dll后缀,程序会根据所属平台自动添加上.so/.dll后缀
	 @return true/false
	 */
	bool loadPlugin(const char* pluginDir, const char* pluginName);

	/**
	 @brief 获取Module的依赖链
	 @param[in] moduleId Module唯一的标识
	 @return Model的依赖链
	 */
	LogicModuleChain* getModuleChain(const int64_t moduleId);

	/**
	 @brief 获取所有插件包含的Module列表
	 @param[out] moduleIds 所有插件中包含的odule列表
	 @return 无
	 */
	void getMouduleIdentitys(std::vector<int64_t>& moduleIds);

	/**
	 @brief 初始化插件信息,从dll/so插件中获取插件中包含的业务模块信息
	 @param[in] pluginName 插件名称
	 @return true/false
	 */
	bool initPluginInfos(const char* pluginName);

	/**
	 @brief 安全模式获取Module对应的插件实体(若指定插件未加载插件则加载)
	 @param[in] moduleId Model标识
	 @return 插件对象实体
	 */
	LogicPluginEntity* ensureGetPluginEntity(const int64_t moduleId);

	/**
	 @brief 获取插件对象实体
	 @param[in] pluginName 插件名称
	 @return 插件对象实体,若插件不存在或插件未加载则返回nullptr
	 */
	LogicPluginEntity* getPluginEntity(const char* pluginName);

	/**
	 @brief 卸载指定插件
	 @param[in] pluginName 插件名称
	 @return 无
	 */
	void unloadPlugin(const char* pluginName);

protected:
	/**
	  @brief 获取包含指定业务模块的插件名称
	  @param[in] moduleId 业务模块的唯一标识
	  @return 业务模块所在的插件名称
	 */
	std::shared_ptr<std::string> getModulePluginName(const int64_t moduleId);

	/**
	 @brief 加载插件实体,并返回插件实体对象
	 @param[in] pluginName 插件名称
	 @param[in] pluginDir  插件所在文件夹
	 @return 插件实体对象
	 */
	LogicPluginEntity* rawLoadPlugin(const char* pluginName, const char* pluginDir);

private:
	std::string pluginDir_{ "" }; // 插件目录
private:
	// 插件实体列表, pluginName<-->LogicPluginEntity
	std::unordered_map<std::string, std::shared_ptr<LogicPluginEntity>> pluginEntityMap_;
private:
	// 插件中包含的信息: 业务模块所在的插件名称, logicModuleId<-->PluginName
	std::unordered_map<int64_t, std::string> modulePluginNameMap_;
	// 插件中包含的信息: 业务模块的依赖链, logicModuleId <--> LogicModuleChain
	std::unordered_map<int64_t, std::shared_ptr<LogicModuleChain>> moduleChainMap_;
};
ZCORE_NS_END
