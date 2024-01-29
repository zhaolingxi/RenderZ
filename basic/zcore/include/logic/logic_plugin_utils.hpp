#pragma once
#include "zcore_sdk.h"
#include <type_traits>
#include "config/config_manager.h"
#include "logic/logic_module_base.h"
#include "logic/logic_module_manager.h"
#include "log4z.h"
#include "aamisc_utils.hpp"
ZCORE_NS_BEGIN
/**
  @brief 安装业务模块插件(单个业务插件实例)
  @param[in] moduleId 业务模块插件的唯一标识
  @param[in] logicManager 业务插件模块管理器
  @param[in] cfgManager   配置管理器
  @param[in] instIdx  业务插件模块实例,同一业务插件可以针对不同的使用常见创建多个实例
  @return true/false
*/
template<class ModuleClass>
bool install_plugin_module(const int64_t moduleId,
	LogicModuleManager* logicManager, ConfigManager* cfgManager, const uint8_t instIdx = 0)
{
	static_assert(std::is_base_of<LogicModuleBase, ModuleClass>::value, "ModuleClass must inherit from LogicModuleBase");
	if (moduleId <= 0 || logicManager == nullptr || cfgManager == nullptr) {
		return false;
	}
	auto logicModule = std::make_shared<ModuleClass>(moduleId, instIdx);
	logicModule->setIdentity(moduleId);
	logicModule->setInstIdx(instIdx);
	// 关联业务模块管理器和配置管理器
	logicModule->setManager(logicManager, cfgManager);
	// 初始化业务模块
	if (!logicModule->initModule()) {
		logicModule->stopModule();
		logicModule->exitModule();
		return false;
	}
	logicManager->addModule(logicModule);
	return true;
}

/**
  @brief 安装业务模块插件(多个业务插件实例)
  @param[in] moduleId 业务模块插件的唯一标识
  @param[in] logicManager 业务插件模块管理器
  @param[in] cfgManager 配置管理器
  @param[in] instCnt 业务插件模块实例数量
  @return true/false
*/
template<class ModuleClass>
bool install_plugin_modules(const int64_t moduleId,
	LogicModuleManager* logicManager, ConfigManager* cfgManager, const uint8_t instCnt = 0)
{
	static_assert(std::is_base_of<LogicModuleBase, ModuleClass>::value, "ModuleClass must inherit from LogicModuleBase");
	bool ret = true;
	for (int instIdx = 0; instIdx < instCnt; instIdx++) {
		ret = install_plugin_module<ModuleClass>(moduleId, logicManager, cfgManager, instIdx);
		if (!ret) {
			LOGFMTE("install plugin module classname: %s, instidx = %d, failed",
				aa_get_tpname<ModuleClass>().c_str(), instIdx);
			break;
		}
	}
	return ret;
}
ZCORE_NS_END
