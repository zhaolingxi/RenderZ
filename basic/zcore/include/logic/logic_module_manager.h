#pragma once
#include "zcore_sdk.h"
#include <map>
#include <vector>
#include <any>
#include <memory>
#include <unordered_map>
#include "config/config_manager.h"
#include "logic_module_base.h"
#include "logic_module_evtdispatcher.h"
/**********************************************************************
 *@file  logic_module_manager.h
 *@date  2023/07/01 15 : 59
 *@author shanwenbin
 *@brief 逻辑模块管理器
 ***********************************************************************/
ZCORE_NS_BEGIN
class LogicModuleBase;
class LogicPluginManager;
class ZCORE_API LogicModuleManager : public LogicModuleEvtDispatcher
{
	friend class AACoreSingleton;
public:
	/**
 	  @brief 获取业务模块实例的唯一标识
 	  @param[in] moduleId 模块类型的唯一标识
 	  @param[in] instIdx 模块实例索引号
 	  @return 业务模块实例的唯一标识
 	 */
	int64_t getModuleInstUID(const int64_t moduleId, const uint8_t instIdx);

	std::shared_ptr<LogicModuleBase>  getModuleByIdentity(const int64_t moduleId, const uint8_t instIdx);

	/**
	 @brief 将Model托管到Model管理器
	 @note 若Model标识重复,则会抛出异常
	 @param[in] model 指定的Model对象
	 @return 无
	 */
	void addModule(std::shared_ptr<LogicModuleBase> logicMod);

	bool installModuleChain(const int64_t muduleId);

protected:
	/**
	 @brief 设置Model管理器关联的插件管理器
	 @param[in] pluginMgr 插件管理器
	 @return 无
	 */
	void setPluginManager(LogicPluginManager* pluginMgr);

	/**
	 @brief 设置Model管理器关联的配置管理器
	 @param[in] configMgr 配置管理器
	 @return 无
	 */
	void setConfigManager(ConfigManager* configMgr);

protected:
	LogicModuleManager() = default;
	virtual ~LogicModuleManager() = default;
protected:
//	AADISABLE_COPY(LogicModuleManager);

private:
	// 插件管理器
	LogicPluginManager* pluginManager_{ nullptr };
	// 配置管理器,业务模块插件和界面公用同一配置管理
	ConfigManager* configManager_{ nullptr };
private:
	// 业务模块的唯一实例标识与模块对象的对应关系, uidInst <---> LogicModuleBase
	std::unordered_map<int64_t, std::shared_ptr<LogicModuleBase>> uidInstToModuleMap_;
};
ZCORE_NS_END
