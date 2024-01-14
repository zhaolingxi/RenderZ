#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include "logic/logic_module_base.h"
#include <functional>
#include <string>
#include <cstdint>
/**********************************************************************
 *@file   aacore_singleton.h
 *@date   2022/11/01 18 : 53
 *@brief  应用程序单例管理中心
 ***********************************************************************/
ZCORE_NS_BEGIN
using ZCORESINGLETON_DESTRORY_CALLBACK = std::function<void()>;
class ILog4zManager;
class ConfigManager;
class ThreadManager;
class SchedulerManager;
class ExecuterManager;
class LogicModuleManager;
class LogicPluginManager;
class ZCORE_API ZCoreSingleton
{
public:
	/**
	 @brief 初始化业务模块
	 @param[in] appInstance 应用程序实例句柄
	 @param[in] appCfgDir   程序配置配置文件路径
	 @param[in] logCfgFilePath 日志配置文件路径
	 @return true:初始化成功, false:初始化失败
	 */
	static bool initCoreFramework(const char* appCfgDir = nullptr, const char* logCfgFilePath = nullptr);

	/**
	  @brief 加载业务模块插件
	  @param[in] pluginDir 插件所在目录
	  @param[in] pluginName 插件名称
	  @return true/false
	 */
	static bool loadPlugin(const char* pluginDir, const char* pluginName);

	/**
	  @brief 安装所有业务模块插件
	  @return true/false
	 */
	static bool installAllModules();

	/**
	  @brief 安装指定标识的业务模块的所有实例
	  @param[in] moduleTag 业务模块标识
	  @return true/false
	 */
	static bool installModuleChain(const char* moduleTag);

	/**
	  @brief 获取指定的业务模块实例
	  @param[in] moduleId 业务模块的唯一标识
	  @param[in] instIdx 业务模块的实例索引编号
	  @return 业务模块实例
	 */
	static std::shared_ptr<LogicModuleBase> getModuleByIdentity(const int64_t moduleId, const uint8_t instIdx = 0);

	template<class TypeInterface>
	static std::shared_ptr<TypeInterface> getModuleByIdCast(const int64_t moduleId, const uint8_t instIdx = 0)
	{
		auto modInterface = getModuleByIdentity(moduleId, instIdx);
		return aasp_dycast<LogicModuleBase, TypeInterface>(modInterface);
	}

	/**
	 @brief 退出管理模块(可能会由于线程等待引起阻塞)
	 @note: 退出时为保证界面显示正常，该按如下顺序:
			1. 退出线程管理模块(会设置程序已退出信号锁)
			2. 等待线程管理模块退出完毕(等待过程中需要继续调度其他模块)
			3. 退出任务管理模块
			4. 退出消息中心
			5. 退出日志管理模块
			6. 退出组件管理模块
	 @param[in] destroyExternalCB 释放外部模块的回调函数
	 @return 无
	 */
	static void exitCoreFramework(ZCORESINGLETON_DESTRORY_CALLBACK destroyExternalCB = nullptr);
	static void singleLoop();

	/**
	 @brief 获取每次事件循环可耗费的最长时间(单位：毫秒)
	 @return 每次事件循环可耗费的最长时间(单位：毫秒)
	 */
	static int getMaxTimePerLoop();
	/**
	 @brief 获取主线程ID
	 @return  主线程ID
	 */
	static unsigned long getMainThreadId();

	/**
	 @brief 是否运行在主线程
	 @return true/false
	 */
	static bool isRunningInMainThread();

	static ConfigManager* getConfigManager();
	static ThreadManager* getThreadManager();
	static ILog4zManager* getLogManager();
	static SchedulerManager* getSchedulerManager();
	static ExecuterManager* getExecuterManager();
	/**
	  @brief 获取业务逻辑模块管理器
	  @return 业务逻辑模块管理器
	 */
	static LogicModuleManager* getLogicModuleManager();

	/**
	  @brief 获取业务逻辑模块插件管理器
	  @return 业务逻辑模块插件管理器
	 */
	static LogicPluginManager* getLogicPluginManager();

#if 0
	// 消息跨进程分发
	static MessageCenter* getMessageCenter();
	static MessagePump* getMessagePump();
#endif

private:
	ZCoreSingleton() = default;
	~ZCoreSingleton() = default;
private:
	static bool kIsExited; // 框架是否已退出 
	static ThreadManager* kThreadManager;
	static ConfigManager* kConfigManager;
	static SchedulerManager* kSchedulerManager;
	static ExecuterManager* kExecuterManager;
	static LogicModuleManager* kLogicModuleManager; /**< 业务逻辑模块管理器 */
	static LogicPluginManager* kLogicPluginManager; /**< 业务逻辑模块插件管理器 */
	static unsigned long kMainThreadId;
};
ZCORE_NS_END
