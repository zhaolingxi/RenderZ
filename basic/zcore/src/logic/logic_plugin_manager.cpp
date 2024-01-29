//#include "logic/logic_plugin_manager.h"
////#include "std::string.h"
//#include "log4z.h"
//ZCORE_NS_BEGIN
//void LogicPluginManager::setPluginDir(const char* pluginDir)
//{
//	if (std::string::isEmptyStr(pluginDir)) {
//		return;
//	}
//	pluginDir_ = pluginDir;
//	if (!aastdstr_endwith(pluginDir_, '/') && !aastdstr_endwith(pluginDir_, '\\')) {
//		pluginDir_ += "/";
//	}
//}
//
//bool LogicPluginManager::isPluginLoaded(const char* pluginName)
//{
//	auto pluginEnity = getPluginEntity(pluginName);
//	return (pluginEnity != nullptr);
//}
//
//bool LogicPluginManager::loadPlugin(const char* pluginDir, const char* pluginName)
//{
//	if (std::string::isEmptyStr(pluginName) || std::string::isEmptyStr(pluginDir)) {
//		return false;
//	}
//	auto pluginEntity = rawLoadPlugin(pluginName, pluginDir);
//	return (pluginEntity != nullptr);
//}
//
//LogicPluginEntity* LogicPluginManager::rawLoadPlugin(const char* pluginName, const char* pluginDir)
//{
//	// 若插件已加载,则直接返回插件实体
//	auto pluginEntity = getPluginEntity(pluginName);
//	if (pluginEntity != nullptr) {
//		return pluginEntity;
//	}
//	//  初始化插件实体
//	auto spPluginEntity = std::make_shared<LogicPluginEntity>();
//	bool ret = spPluginEntity->initEntity(pluginName, pluginDir);
//	if (!ret) {
//		spPluginEntity->exitEntity();
//		return nullptr;
//	}
//	pluginEntityMap_.insert(std::make_pair(pluginName, spPluginEntity));
//	pluginEntity = spPluginEntity.get();
//	return pluginEntity;
//}
//
//LogicModuleChain* LogicPluginManager::getModuleChain(const int64_t moduleId)
//{
//	auto it = moduleChainMap_.find(moduleId);
//	if (it == moduleChainMap_.end()) {
//		return nullptr;
//	}
//	return it->second.get();
//}
//
//void LogicPluginManager::getMouduleIdentitys(std::vector<int64_t>& moduleIds)
//{
//	moduleIds.clear();
//	auto it = modulePluginNameMap_.begin();
//	for (; it != modulePluginNameMap_.end(); ++it) {
//		moduleIds.push_back(it->first);
//	}
//}
//
//bool LogicPluginManager::initPluginInfos(const char* pluginName)
//{
//	auto pluginEntity = getPluginEntity(pluginName);
//	if (pluginEntity == nullptr) {
//		return false;
//	}
//	auto pluginModuleInfo = std::make_shared<LogicPluginModuleInfo>();
//	// 获取插件dll/so中包含的View信息
//	pluginEntity->getModuleInfo(pluginModuleInfo.get());
//	for (int i = 0; i < pluginModuleInfo->getModuleCnt(); i++) {
//		int64_t moduleId = pluginModuleInfo->getModuleIdentity(i);
//		// 构造Module与插件名称的关联关系,不带.so或.dll后缀
//		modulePluginNameMap_.insert(std::make_pair(moduleId, pluginEntity->getName()));
//		// 获取Module所依赖的基础业务模块
//		auto moduleChain = pluginModuleInfo->getModuleChainByIdentity(moduleId);
//		if (moduleChain != nullptr) {
//			moduleChainMap_.insert(std::make_pair(moduleId, moduleChain));
//		}
//	}
//	return true;
//}
//
//LogicPluginEntity* LogicPluginManager::ensureGetPluginEntity(const int64_t moduleId)
//{
//	auto pluginName = getModulePluginName(moduleId);
//	if (pluginName == nullptr) {
//		return nullptr;
//	}
//	auto pluginEnity = getPluginEntity(pluginName->c_str());
//	if (pluginEnity != nullptr) {
//		return pluginEnity;
//	}
//	// 从默认路径加载插件
//	return rawLoadPlugin(pluginName->c_str(), pluginDir_.c_str());
//}
//
//std::shared_ptr<std::string> LogicPluginManager::getModulePluginName(const int64_t moduleId)
//{
//	auto it = modulePluginNameMap_.find(moduleId);
//	if (it == modulePluginNameMap_.end()) {
//		return nullptr;
//	}
//	auto pluginName = std::make_shared<std::string>(it->second);
//	return pluginName;
//}
//
//LogicPluginEntity* LogicPluginManager::getPluginEntity(const char* pluginName)
//{
//	auto it = pluginEntityMap_.find(pluginName);
//	if (it == pluginEntityMap_.end()) {
//		return nullptr;
//	}
//	return it->second.get();
//}
//
//void LogicPluginManager::unloadPlugin(const char* pluginName)
//{
//	if (std::string::isEmptyStr(pluginName)) {
//		return;
//	}
//	auto it = pluginEntityMap_.find(pluginName);
//	if (it == pluginEntityMap_.end()) {
//		return;
//	}
//	it->second->exitEntity();
//	it->second = nullptr;
//	pluginEntityMap_.erase(it);
//	LOGFMTI("unloadPlugin() success, pluginName: %s", pluginName);
//}
//ZCORE_NS_END
