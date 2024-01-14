//#include "logic/logic_module_manager.h"
//#include "logic/logic_module_base.h"
//#include "logic/logic_plugin_manager.h"
//#include "log4z.h"
//ZCORE_NS_BEGIN
//void LogicModuleManager::setPluginManager(LogicPluginManager* pluginMgr)
//{
//	pluginManager_ = pluginMgr;
//}
//
//void LogicModuleManager::setConfigManager(ConfigManager* configMgr)
//{
//	configManager_ = configMgr;
//}
//
//int64_t LogicModuleManager::getModuleInstUID(const int64_t moduleId, const uint8_t instIdx)
//{
//	return (moduleId | instIdx);
//}
//
//std::shared_ptr<LogicModuleBase> LogicModuleManager::getModuleByIdentity(const int64_t moduleId, const uint8_t instIdx)
//{
//	auto uidInst = getModuleInstUID(moduleId, instIdx);
//	auto it = uidInstToModuleMap_.find(uidInst);
//	if (it == uidInstToModuleMap_.end()) {
//		return nullptr;
//	}
//	return it->second;
//}
//
//void LogicModuleManager::addModule(std::shared_ptr<LogicModuleBase> logicMod)
//{
//	int64_t uidInst = getModuleInstUID(logicMod->getIdentity(), logicMod->getInstIdx());
//	auto it = uidInstToModuleMap_.find(uidInst);
//	if (it != uidInstToModuleMap_.end()) {
//		LOGFMTE("addModule() module inst existed, replace by new module identity = %lld, instIdx = %d",
//			logicMod->getIdentity(), logicMod->getInstIdx());
//		it->second = logicMod;
//	}
//	uidInstToModuleMap_.insert(std::make_pair(uidInst, logicMod));
//}
//
//bool LogicModuleManager::installModuleChain(const int64_t moduleId)
//{
//	if (moduleId <= 0) {
//		LOGFMTE("installModuleChain invalid muduleId = %lld", moduleId);
//		return false;
//	}
//	if (pluginManager_ == nullptr || configManager_ == nullptr) {
//		LOGFMTE("installModelChain() Need Set pluginManager, configManager");
//		return false;
//	}
//	// Module已成功安装,相同类型的多个实例是同时创建的
//	if (getModuleByIdentity(moduleId, 0) != nullptr) {
//		return true;
//	}
//	// 获取Model对应的插件实体,若插件未加载则自动加载
//	auto pluginEntity = pluginManager_->ensureGetPluginEntity(moduleId);
//	if (pluginEntity == nullptr) {
//		return false;
//	}
//	// 获取Model的依赖
//	auto modelChain = pluginManager_->getModuleChain(moduleId);
//	// 不存在依赖, 则直接安装ModuleManager
//	// pluginEntity->installModule()会自动将View添加到ModuleManager
//	if (modelChain == nullptr) {
//		return pluginEntity->installModule(moduleId, this, configManager_);
//	}
//	// 创建依赖的Module
//	bool ret = true;
//	for (int i = 0; i < modelChain->getDependModuleCnt(); i++) {
//		int64_t dependModId = modelChain->getDependModuleId(i);
//		ret = installModuleChain(dependModId);
//		if (!ret) {
//			break;
//		}
//	}
//	if (!ret) {
//		return false;
//	}
//	// 安装业务模块
//	return pluginEntity->installModule(moduleId, this, configManager_);
//}
//ZCORE_NS_END
