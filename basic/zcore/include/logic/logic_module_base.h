#pragma once
#include "zcore_sdk.h"
#include "config/config_base.h"
//#include "task_executer.h"
#include "logic_module_notify_router.h"
#include "config/config_decorator.h"
#include <cstdint>
#include <cstdio>
/**********************************************************************
 *@file  logic_module_base.h
 *@date  2023/07/01 14 : 30
 *@author shanwenbin
 *@brief 业务模块插件基类
 *       1. 不允许在初始化模块和退出模块的过程中发送消息或者post任务
 ***********************************************************************/
ZCORE_NS_BEGIN
// 业务模块基类
// TaskExecuter 修改成类成员
class LogicModuleManager;
class ZCORE_API LogicModuleBase : public LogicModuleNotifyRouter
{
public:
	LogicModuleBase();
	virtual ~LogicModuleBase();
public:
	/**
	  @brief 设置模块管理器和配置管理器
	  @param[in] moduleManager 模块管理器
	  @param[in] configManager 配置管理器
	  @return 无
	 */
	void setManager(LogicModuleManager* moduleManager, ConfigManager* configManager);

	/**
	 @brief 获取指定名称的配置
	 @param[in] pluginName 插件名称,配置的唯一标识
	 @return 模块配置信息
	 */
	std::shared_ptr<ConfigBase> getConfigByName(const char* pluginName);
	template<typename T>
	std::shared_ptr<T> getCfgDecoratorByName(const char* pluginName) {
		static_assert(std::is_base_of<ConfigDecorator, T>::value, "T must Derived ConfigDecorator");
		auto cfg = getConfigByName(pluginName);
		if (cfg == nullptr) {
			return nullptr;
		}
		auto decorator = std::make_shared<T>(cfg);
		decorator->parseConfig();
		return decorator;
	}

	/**
	  @brief 获取指定identity的业务模块
	  @param[in] moduleId 业务模块的唯一标识
	  @param[in] idx 附件索引号,同一业务模块可能存在多个实例
	  @return 业务模块对象
	 */
	std::shared_ptr<LogicModuleBase> getModuleByIdentity(const int64_t moduleId, const uint8_t idx = 0);

	template<typename T>
	std::shared_ptr<T> getModuleByIdCast(const int64_t moduleId, const uint8_t idx = 0)
	{
		auto logicMod = getModuleByIdentity(moduleId, idx);
		return aasp_dycast<LogicModuleBase, T>(logicMod);
	}

public:
	/**
	  @brief 设置模块的唯一标识
	  @param[in] modIdentity 模块的唯一标识
	  @return 无
	 */
	void setIdentity(const int64_t modIdentity);

	/**
	  @brief 获取逻辑模块的唯一标识
	  @return 逻辑模块的唯一标识
	 */
	int64_t getIdentity() const;

	/**
	  @brief 设置实例索引号
	  @param[in] instIdx 实例索引号
	  @return 无
	 */
	void setInstIdx(const uint8_t instIdx);

	/**
	  @brief 获取逻辑模块的实例索引号
	  @return 逻辑模块的实例索引号
	 */
	uint8_t getInstIdx() const;

public:
	/**
	 @brief 初始化业务模块配置,需要在initModule中或initModule前调用该接口
	 @param[in] config 业务插件模块的配置对象
	 @return 无
	 */
	virtual void setConfig(std::shared_ptr<ConfigBase> config);

public:
	/**
	 @brief 获取配置
	 @return 配置对象
	 */
	std::shared_ptr<ConfigBase> getConfig();
	template<typename T>
	std::shared_ptr<T> getConfigCast() {
		return aasp_stcast<ConfigBase, T>(getConfig());
	}

public:
	/**
	 @brief 初始化业务模块
	 @return true/false
	 */
	virtual bool initModule();

	/**
	 @brief 停止业务模块
	 @note 停止定时任务和额外的任务线程,解除模块间的关联
	 @return 无
	 */
	virtual void stopModule();

	/**
	 @brief 退出业务模块
	 @note 释放关联对象的空间
	 @return 无
	 */
	virtual void exitModule();

public:
	/**
 	  @brief 发送异步请求到Executer绑定的任务调度器
	  @note 响应回调 ackCB 将会在asyncPeer绑定的任务调度器中执行
 	  @param[in] asyncPeer 异步请求对象
	  @param[in] ackCB 异步响应回调
	  @param[in] delayTimeMs 延时时间(毫秒)
 	  @return true/false
 	 */
	//bool sendModuleAsyncRequest(std::shared_ptr<AsyncPeer>& asyncPeer, AsyncPeer::AckCB ackCB, const int delayTimeMs = 0);

	/**
 	  @brief 发送异步请求到Executer绑定的任务调度器
 	  @param[in] asyncPeer 异步请求对象
	  @param[in] delayTimeMs 延时时间(毫秒)
 	  @return true/false
 	 */
	//bool sendModuleAsyncRequest(std::shared_ptr<AsyncPeer>& asyncPeer, const int delayTimeMs = 0);

protected:
	/**
	  @brief 绑定任务调度器
	  @param[in] taskScheduler 任务调度器对象
	  @return true/false
	 */
	//bool bindTaskScheduler(TaskScheduler* taskScheduler);

	/**
	  @brief 获取模块绑定的任务执行器
	  @return 模块绑定的任务执行器
	 */
	//TaskScheduler* getBindTaskScheduler();

	/**
	  @brief 获取关联的任务调度器
	  @return 关联的任务调度器
	 */
	//std::shared_ptr<TaskExecuter> getTaskExecuter();

	/**
 	  @brief 绑定命令的异步处理回调(子线程中异步执行)
 	  @param[in] cmdId 命令ID
 	  @param[in] asyncCB 异步处理回调
 	  @return true/false
 	 */
	//bool bindModuleAsyncProcCB(const int64_t cmdId, ExecuterAsyncProcCB asyncCB);

	/**
 	  @brief 发送异步响应到AsyncPeer绑定的Ack调度器
 	  @param[in] asyncPeer 异步请求对象
 	  @return true/false
 	 */
	//bool sendModuleAsyncResp(std::shared_ptr<AsyncPeer>& asyncPeer);

	/**
 	  @brief 发送异步任务结果到AsyncPeer绑定的Ack调度器
 	  @param[in] asyncPeer 异步请求对象
 	  @return true/false
 	 */
	//bool sendModuleAsyncResult(std::shared_ptr<AsyncPeer> asyncPeer, AsyncPeer::AckCB ackCB = nullptr);

protected:
	/**
	  @brief 在主线程中运行异步任务
	  @note 使用场景较少,可以用于同一操作多次重试或实现运行在主线程的定时器
	  @param[in] asyncCB 异步任务回调
	  @param[in] delayTimeMs 异步回调的延迟时间
	  @return true/false
	 */
	bool runAsyncTaskInMain(std::function<void()> asyncCB, const int delayTimeMs = 0);

	/**
	  @brief 在子线程中运行异步任务
	  @note 可以用于同一操作多次重试或实现运行在子线程的定时器
	  @param[in] asyncCB 异步任务回调
	  @param[in] delayTimeMs 异步回调的延迟时间
	  @return true/false
	 */
	bool runAsyncTaskInChild(std::function<void()> asyncCB, const int delayTimeMs = 0);

	/**
	  @brief 以阻塞模式执行异步任务
	  @param[in] blockTaskCB 阻塞任务回调
	  @return 成功: 返回0,失败返回 blockTaskCB 中返回的错误码
	 */
	int64_t runAsyncTaskBlocked(std::function<int64_t()> blockTaskCB);

private:
	LogicModuleManager* moduleManager_{ nullptr }; // 业务模块管理器
	ConfigManager* configManager_{ nullptr }; // 配置管理器
private:
	int64_t moduleIdentity_{ 0 }; // Model的唯一标识
	uint8_t instIdx_{ 0 }; // 业务模块的实例索引号,同一模块可以创建多个实例
	std::shared_ptr<ConfigBase> config_{ nullptr }; // 关联的配置对象
	//std::shared_ptr<TaskExecuter> taskExecuter_{ nullptr }; // 异步任务执行器
};
ZCORE_NS_END
