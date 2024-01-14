#pragma once
#include "zcore_sdk.h"
#include <cstdint>
#include <functional>
#include <atomic>
#include "delaytask_queue.h"
//#include "thread_base.h"
/**********************************************************************
 *@file   task_scheduler.h
 *@date   2022/10/26 18 : 25
 *@brief 基于Asio和无锁队列实现的异步任务调度器
		 Note: 当前仅实现asio部分,无锁队列的实现后面再扩展
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API TaskScheduler
{
public:
	// 任务调度器的使用场景(界面Model、数据库读写)
	enum class SchedScope : int {
		kGenral = 0,   /**< 通用调度器 */
		kLogicModule = 1, /**< 供逻辑模块使用的调度器 */
		kWSServer = 2,   /**< 供Websocket服务端使用的调度器 */
		kWSClient = 3,   /**< 供Websocket服务端使用的调度器 */
		kHttpServer = 4, /**< 用于Http服务端的调度器(暂未使用) */
		kServiceModel = 5,    /**< 供Websocket服务端ServiceModel使用的调度器  */
		kDatabase = 6,   /**< 用于数据库读写的调度器 */
		kMax
	};

	enum class SchedType : int {
		kNone = 0, /**< 无 */
		kNonThread = 1, /**< 不带线程的任务调度器(运行在主线程) */
		kThread = 2, /**< 带线程的任务调度器(运行在子线程) */
	};
public:
	TaskScheduler(SchedType type, SchedScope scope = SchedScope::kGenral);
	virtual ~TaskScheduler();

public:
	/**
	  @brief 获取调度器的唯一标识
	  @note 对于带子线程的调度器,使用线程ID作为调度器的唯一标识
	  @return 调度器的唯一标识
	 */
	uint64_t getIdentity();

	SchedScope getScope();
	SchedType getType();

	/**
	  @brief 获取正在执行的任务数量
	  @return 正在执行的任务数量
	 */
	int getRunningTaskCnt();

	/**
	  @brief 获取异步调度上下文
	  @return 异步调度上下文
	 */
	template<typename T>
	T& getAsyncIOContext()
	{
		T& context = *(static_cast<T*>(asyncIOContext_));
		return context;
	}

	uint64_t getSchedThreadId();

	bool init();

	void exit();

	bool start();

	void stop();

	void waitStop();

	/**
	 @brief 任务调度服务是否已停止
	 @return true/false
	 */
	bool isStopped();

	/**
	  @brief 执行调度队列中的任务
	  @param[in] tickTime 当前时间
	  @param[in] isPollAll 是否执行调度队列中的所有任务
	  @return 是否需要增加singleStep的调用频率
	 */
	bool singleStep(const uint64_t tickTime, const bool isPollAll = false);

	/**
	  @brief 添加异步任务到调度队列(可以跨线程调用)
	  @param[in] asyncCallback 异步任务回调
	  @return true/false
	 */
	bool postTask(std::function<void()> asyncCallback);

	/**
	  @brief 添加定时任务到调度队列(可以跨线程调用)
	  @param[in] asyncCallback 异步任务回调
	  @param[in] delayTimeMs 等待时间
	  @return true/false
	 */
	bool postDelayTask(std::function<void(const int errCode)> asyncCallback, const int delayTimeMs);

	/**
	  @brief 添加异步任务到调度队列(只允许在调度线程中调用)
	  @param[in] asyncCallback 异步任务回调
	  @return true/false
	 */
	bool postInnerTask(std::function<void()> asyncCallback);

	/**
	  @brief 添加定时任务到调度队列(只允许在调度线程中调用)
	  @param[in] asyncCallback 异步任务回调
	  @param[in] delayTimeMs 等待时间
	  @return true/false
	 */
	bool postInnerDelayTask(std::function<void(const int errCode)> asyncCallback, const int delayTimeMs);

public:
	/**
	  @brief 获取运行在主线程的任务调度器
	  @return 调度器对象
	 */
	static TaskScheduler* getMainThreadScheduler();

	/**
	  @brief 获取业务模块公用的任务调度器
	  @return 业务模块公用的任务调度器
	 */
	static TaskScheduler* getCommLogicScheduler();

	/**
	  @brief 获取服务模块ServiceModel公用的任务调度器
	  @return 服务模块ServiceModel公用的任务调度器
	 */
	static TaskScheduler* getCommServiceModelScheduler();

	/**
	  @brief 创建不带子线程的任务调度器
	  @param[in] scope 调度器的作用范围
	  @return 调度器对象
	 */
	static TaskScheduler* createNonThreadScheduler(SchedScope scope = SchedScope::kGenral);

	/**
	  @brief 创建带子线程的任务调度器
	  @param[in] scope 调度器的作用范围
	  @return 调度器对象
	 */
	static TaskScheduler* createThreadScheduler(SchedScope scope = SchedScope::kGenral);

	/**
	  @brief 销毁任务调度器
	  @param[in] scheduler 调度器对象
	  @return 无
	 */
	static void destroyScheduler(TaskScheduler* scheduler);

	/**
	  @brief 获取系统启动时预设的任务调度器--待完善
	 */
	static TaskScheduler* getSchedulerByScope(SchedScope scope);
	static void releaseScheduler(TaskScheduler* scheduler);

protected:

	/**
	 @brief 启动调度子线程(仅用于ThreadTaskService)
	 @return 成功:线程ID, 失败: -1
	 */
	virtual unsigned long startShedThread();

	/**
	 @brief 停止调度子线程(仅用于ThreadTaskService)
	 @return 成功:线程ID, 失败: -1
	 */
	virtual void stopShedThread();

	/**
	 @brief 等待调度子线程停止完成(仅用于ThreadTaskService)
	 @return 成功:线程ID, 失败: -1
	 */
	virtual void waitShedThreadStop();

	/**
	 @brief 服务调度线程是否已退出
	 @return 是/否
	 */
	virtual bool isShedThreadStopped();

private:
	// 不允许对象拷贝
	TaskScheduler(const TaskScheduler& other) = delete;
	void operator=(const TaskScheduler& other) = delete;
	TaskScheduler(const TaskScheduler&& other) = delete;
	void operator=(const TaskScheduler&& other) = delete;
private:
	SchedScope scope_{ SchedScope::kGenral };
	SchedType schedType_{ SchedType::kNone };
	bool isRunning_{ false }; /**< 调度服务是否处于运行中 */
	std::atomic<int> runningTaskCnt_{ 0 }; /**< 正在执行的任务数量 */
	void* asyncIOContext_{ nullptr }; /**< 基础IO异步服务 */
	unsigned long schedThreadId_{ 0 }; /**< 负责调度任务的线程ID */
	DelayTaskQueue delayTaskQueue_; /**<  延时任务队列 */
//	ThreadBase schedThread_; /**< 调度线程 */
};
ZCORE_NS_END
