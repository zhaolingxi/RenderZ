#pragma once
#include "zcore_sdk.h"
#include <cstdint>
#include <functional>
#include <atomic>
#include "task_queue.h"
#include "zstring.h"
#include "zlockfree_queue.hpp"
#include"zthread.h"
/**********************************************************************
 *@brief 异步任务调度器(后续版本需要将执行器分离出来)
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
#define ThreadQueue zutils::ZLockFreeQueue<zutils::ZThread>

public:
	TaskScheduler(SchedType type,int workerNum=-1);
	virtual ~TaskScheduler()=default;

public:
	/**
	  @brief 获取调度器的唯一标识（	UUID格式与生成方法还没有确定）
	  @note 对于带子线程的调度器,使用线程ID作为调度器的唯一标识
	  @return 调度器的唯一标识
	 */
	//uint64_t getIdentity();


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

	void schedule();//调度线程函数，使用调度线程进行线程的任务分配

	void start();//启动调度器，会调用当前空闲的线程执行准备好的任务
	void pause();//暂停调度器
	void stopNow();//终止调度器，将会导致任务队列清空，所有线程挂起(立即执行)
	void waitStop();//终止调度器，将会导致任务队列清空，所有线程挂起（等待当前的执行的线程完成）
	void exit();//释放所有的资源，释放完之后请记得删除本调度器

	/**
	 @brief 任务调度服务是否已停止
	 @return true/false
	 */
	bool isStopped();

	/**
	  @brief 单步轮询（使用timer）调度器通过timer来执行延时任务
	  @param[in] tickTime 当前时间
	  @param[in] isPollAll 是否执行调度队列中的所有任务
	  @return 是否需要增加singleStep的调用频率
	 */
//	bool singleStep(const uint64_t tickTime, const bool isPollAll = false);

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
	bool postDelayTask(std::function<void()> asyncCallback, const int delayTimeMs);

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
	std::shared_ptr <TaskQueue> taskQueue_{ nullptr }; /**<  延时任务队列 */
	std::shared_ptr<zutils::ZThread> schedThread_{nullptr}; /**< 调度线程 */
	int threadNum_{0};
	std::deque<zutils::ZThread*> threadQueue_;//后面需要修改为无锁队列 
	std::deque<zutils::ZThread*> threadBusyQueue_;//后面需要修改为无锁队列 
};
ZCORE_NS_END
