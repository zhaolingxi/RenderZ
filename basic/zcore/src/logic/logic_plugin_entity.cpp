//#include "logic/logic_plugin_entity.h"
//#include "log4z.h"
//#include "zutils.h"
//ZCORE_NS_BEGIN
//const std::string& LogicPluginEntity::getName() const
//{
//	return name_;
//}
//
//const std::string& LogicPluginEntity::getFileDir() const
//{
//	return fileDir_;
//}
//
//const std::string& LogicPluginEntity::getVersion() const
//{
//	return version_;
//}
//
//const std::string& LogicPluginEntity::getManufacture() const
//{
//	return manufacture_;
//}
//
//std::shared_ptr<std::string> LogicPluginEntity::getPluginFilePath(const std::string& pluginName, const std::string& pluginDir)
//{
//	auto pluginFilePath = std::make_shared<std::string>(pluginDir);
//	if (!aastdstr_endwith(*pluginFilePath, '/') && !aastdstr_endwith(*pluginFilePath, '\\')) {
//#if defined(WIN32) || defined(WIN64)
//		pluginFilePath->append("\\");
//#else
//		pluginFilePath->append("/");
//#endif
//	}
//#if defined(WIN32) || defined(WIN64)
//	pluginFilePath->append(pluginName);
//	pluginFilePath->append(".dll");
//#else
//	pluginFilePath->append("lib");
//	pluginFilePath->append(pluginName);
//	pluginFilePath->append(".so");
//#endif
//	return pluginFilePath;
//}
//
//bool LogicPluginEntity::initEntity(const std::string& pluginName, const std::string& pluginDir)
//{
//	auto pluginFilePath = getPluginFilePath(pluginName, pluginDir);
//	if (!AAFileUtils::isFileExit(pluginFilePath->c_str()) || pluginName.empty()) {
//		LOGFMTE("initEntity pluginFile not exist, pluginFilePath: %s", pluginFilePath->c_str());
//		return false;
//	}
//
//	// 加载插件
//	std::shared_ptr<ZLibLoader> pluginLib = std::make_shared<ZLibLoader>(pluginFilePath->c_str());
//	if (!pluginLib->load()) {
//		LOGFMTE("initEntity load plugin failed, pluginFilePath: %s", pluginFilePath->c_str());
//		return false;
//	}
//
//	// 初始化插件入口函数
//	getPluginModuleInfo_ = (EntryGetPluginModuleInfo)pluginLib->resolve("GetLogicPluginModuleInfo");
//	installPluginModule_ = (EntryInstallPluginModule)pluginLib->resolve("InstallLogicPluginModule");
//	// 初始化实例成员
//	pluginLibrary_ = pluginLib;
//	fileDir_ = pluginDir;
//	name_ = pluginName;
//	LOGFMTD("initEntity() success pluginName:%s, getPluginModuleInfo_=%lld, installPluginModule_=%lld",
//		getName().c_str(),
//		reinterpret_cast<int64_t>(getPluginModuleInfo_),
//		reinterpret_cast<int64_t>(installPluginModule_));
//	return true;
//}
//
//void LogicPluginEntity::exitEntity()
//{
//	if (pluginLibrary_ != nullptr) {
//		if (pluginLibrary_->isLoaded()) {
//			pluginLibrary_->unload();
//		}
//		pluginLibrary_ = nullptr;
//		getPluginModuleInfo_ = nullptr;
//		installPluginModule_ = nullptr;
//	}
//	fileDir_ = ""; // 插件文件所在文件夹
//	name_ = ""; ; // 插件名称
//	version_ = "";// 插件版本
//	manufacture_ = ""; // 生产商
//}
//
//bool LogicPluginEntity::getModuleInfo(LogicPluginModuleInfo* pluginModuleInfo)
//{
//	if (getPluginModuleInfo_ == nullptr || pluginModuleInfo == nullptr) {
//		return false;
//	}
//	getPluginModuleInfo_(pluginModuleInfo);
//	return true;
//}
//
//bool LogicPluginEntity::installModule(const int64_t moduleId, LogicModuleManager* logicMgr, ConfigManager* configMgr)
//{
//	if (installPluginModule_ == nullptr || moduleId <= 0) {
//		return false;
//	}
//	return installPluginModule_(moduleId, logicMgr, configMgr);
//}
//
//void LogicPluginEntity::uninstallModule(const int64_t modelId)
//{
//	// 待完善
//}
//ZCORE_NS_END
