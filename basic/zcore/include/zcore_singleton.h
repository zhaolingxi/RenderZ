#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include <functional>
#include <string>
#include <cstdint>
/**********************************************************************
 *@brief  应用程序单例管理中心
 ***********************************************************************/
ZCORE_NS_BEGIN
using ZCORESINGLETON_DESTRORY_CALLBACK = std::function<void()>;

class ILog4zManager;
class ConfigManager;
class LogicModuleManager;
class LogicPluginManager;
class ZCORE_API ZCoreSingleton
{
public:


	static bool initCoreFramework(const char* appCfgDir = nullptr, const char* logCfgFilePath = nullptr);

	static bool loadPlugin(const char* pluginDir, const char* pluginName);


	static bool installAllModules();

	static bool installModuleChain(const char* moduleTag);


	// template<class TypeInterface>
	// static std::shared_ptr<TypeInterface> getModuleByIdCast(const int64_t moduleId, const uint8_t instIdx = 0)
	// {
	// 	auto modInterface = getModuleByIdentity(moduleId, instIdx);
	// 	return aasp_dycast<LogicModuleBase, TypeInterface>(modInterface);
	// }

	static void exitCoreFramework(ZCORESINGLETON_DESTRORY_CALLBACK destroyExternalCB = nullptr);
	static void singleLoop();

	static ConfigManager* getConfigManager();
	static ILog4zManager* getLogManager();

	static LogicModuleManager* getLogicModuleManager();

	static LogicPluginManager* getLogicPluginManager();


private:
	ZCoreSingleton() = default;
	~ZCoreSingleton() = default;
private:
	static ConfigManager* kConfigManager;
	static LogicModuleManager* kLogicModuleManager; /**< 业务逻辑模块管理器 */
	static LogicPluginManager* kLogicPluginManager; /**< 业务逻辑模块插件管理器 */
	static unsigned long kMainThreadId;
};
ZCORE_NS_END
