#pragma once
#include "zcore_sdk.h"
#include <cstdint>
#include <functional>
#include <atomic>
#include "task_queue.h"
#include"zthread.h"
/**********************************************************************
 *@brief 异步任务调度器
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API TaskScheduler
{
public:
	enum class SchedType : int {
		PRIOR_Sched = 0,//优先级调度
		FIFO_Sched,//先入先出调度
		WEB_TASK_Sched,//网络任务调度
	};

public:
	TaskScheduler(SchedType type,int workerNum=-1);
	virtual ~TaskScheduler()=default;

public:
	/**
	  @brief 获取调度器的唯一标识
	  @note 对于带子线程的调度器,使用线程ID作为调度器的唯一标识
	  @return 调度器的唯一标识
	 */
	uint64_t getIdentity();


	SchedType getType();

	/**
	  @brief 获取正在执行的任务数量
	  @return 正在执行的任务数量
	 */
	int getRunningTaskCnt();

	/**
	  @brief 获取异步调度上下文（WEB_TASK_Sched）
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
	  @brief 销毁任务调度器
	  @param[in] scheduler 调度器对象
	  @return 无
	 */
	static void destroyScheduler(TaskScheduler* scheduler);

	static void releaseScheduler(TaskScheduler* scheduler);


private:
	// 不允许对象拷贝
	TaskScheduler(const TaskScheduler& other) = delete;
	void operator=(const TaskScheduler& other) = delete;
	TaskScheduler(const TaskScheduler&& other) = delete;
	void operator=(const TaskScheduler&& other) = delete;
private:
	SchedType schedType_{ SchedType::PRIOR_Sched };
	bool isRunning_{ false }; /**< 调度服务是否处于运行中 */
	std::atomic<int> runningTaskCnt_{ 0 }; /**< 正在执行的任务数量 */
	void* asyncIOContext_{ nullptr }; /**< 基础IO异步服务 */
	std::shared_ptr < TaskQueue> taskQueue_{ nullptr }; /**<  延时任务队列 */
	std::shared_ptr<zutils::ZThread> schedThread_{nullptr}; /**< 调度线程 */
	int threadNum_{0};
};
ZCORE_NS_END
