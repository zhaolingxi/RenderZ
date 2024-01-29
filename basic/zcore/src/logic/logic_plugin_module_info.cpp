//#include "logic/logic_plugin_module_info.h"
//#include "moduleid_tratis64.hpp"
//#include "log4z.h"
//ZCORE_NS_BEGIN
//void LogicPluginModuleInfo::addModuleIdentity(const int64_t identity)
//{
//	std::string modTag = module_uppertag64(identity);
//	LOGFMTD("addModuleIdentity() identity = %lld, tag: %s", identity, modTag.c_str());
//	auto it = std::lower_bound(moduleIdentitys_.begin(), moduleIdentitys_.end(), identity);
//	// 待添加的Module已经存在
//	if (it != moduleIdentitys_.end() && *it == identity) {
//		LOGFMTE("Error, identity Existed! identity= %lld, tag: %s", identity, modTag.c_str());
//		return;
//	}
//	moduleIdentitys_.insert(it, identity);
//}
//
//void LogicPluginModuleInfo::addDependModule(const int64_t moduleId, const int64_t baseModuleId)
//{
//	ensureGetModuleChain(moduleId)->addDependModuleId(baseModuleId);
//}
//
//int LogicPluginModuleInfo::getModuleCnt()
//{
//	return moduleIdentitys_.size();
//}
//
//int64_t LogicPluginModuleInfo::getModuleIdentity(const int idxModule)
//{
//	if (idxModule < 0 || idxModule >= moduleIdentitys_.size()) {
//		LOGFMTE("Module idxModule out of range, idx = %d", idxModule);
//		throw std::runtime_error("Module idxModule out of range");
//	}
//	return moduleIdentitys_[idxModule];
//}
//
//std::shared_ptr<LogicModuleChain> LogicPluginModuleInfo::getModuleChainByIdx(const int moduleIdx)
//{
//	int64_t identity = getModuleIdentity(moduleIdx);
//	return getModuleChainByIdentity(identity);
//}
//
//std::shared_ptr<LogicModuleChain> LogicPluginModuleInfo::getModuleChainByIdentity(const int64_t moduleId)
//{
//	auto it = moduleChainMap_.find(moduleId);
//	if (it == moduleChainMap_.end()) {
//		return nullptr;
//	}
//	return it->second;
//}
//
//std::shared_ptr<LogicModuleChain>& LogicPluginModuleInfo::ensureGetModuleChain(const int64_t moduleId)
//{
//	auto it = moduleChainMap_.find(moduleId);
//	if (it == moduleChainMap_.end()) {
//		auto ModuleChain = std::make_shared<LogicModuleChain>();
//		ModuleChain->setModuleIdentity(moduleId);
//		auto ret = moduleChainMap_.insert(std::make_pair(moduleId, ModuleChain));
//		return ret.first->second;
//	}
//	return it->second;
//}
//ZCORE_NS_END
