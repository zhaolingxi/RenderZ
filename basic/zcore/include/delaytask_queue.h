#pragma once
#include "zcore_sdk.h"
#include <queue>
#include <vector>
#include <list>
#include <functional>
/**********************************************************************
 *@file   delaytask_queue.h
 *@date   2022/10/26 18 : 25
 *@brief  延时任务队列
		  1、为减少频繁创建任务对象的内存开销,已存在的任务对象可以循环使用
		  2、所有任务按小顶堆的方式存储,时间值越小的位于堆顶
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API DelayTaskQueue
{
public:
	// 延时任务回调函数
	using DELAY_TASKFN = std::function<void(const int errCode)>;

	// 延时任务
	struct DelalyTask
	{
		DELAY_TASKFN taskFn; // 需要执行的任务回调函数
		uint64_t tickTime;   // 延时任务需要执行的时刻
	};
	// 延时任务比较函数
	struct DelalyTaskCompare
	{
		bool operator()(const DelalyTask* x, const DelalyTask* y) {
			return x->tickTime > y->tickTime;
		}
	};

public:
	DelayTaskQueue();
	virtual ~DelayTaskQueue();

public:
	/**
	 @brief 初始化延时任务队列
	 @param[in] maxCapcity 队列最大可容纳的延迟任务数(避免异常操作导致内存占用过高)
	 @return true/false
	 */
	bool initDelayTaskQueue(const int maxCapcity = 1024);

	/**
	 @brief 退出延时任务队列
	 @return 无
	 */
	void exitDelayTaskQueue();

	/**
	 @brief 添加延时任务
	 @param[in] taskFn 延时任务的异步回调函数
	 @param[in] delayTimeMs 任务的执行延时时间
	 @return true - 添加成功, false - 超过最大容量限制
	 */
	bool addDelalyTask(DELAY_TASKFN taskFn, const int delayTimeMs);

	/**
	 @brief 执行已到达的异步延时任务
	 @param[in] tickTime 当前系统时间
	 @return 本轮操作执行的任务数量
	 */
	size_t completeReadyTasks(uint64_t tickTimeMs);

protected:
	/**
	 @brief 获取空闲的任务对象
	 @return 空闲的任务对象
	 */
	DelalyTask *getTaskObjFromIdleList();

	/**
	 @brief 释放空闲任务对象的使用权
	 @param[in] taskObj 空闲的任务对象
	 @return 无
	 */
	void recyleTaskObjToIdleList(DelalyTask *taskObj);

	/**
	 @brief 销毁空闲任务对象列表
	 @return 无
	 */
	void destroyIdleTaskObjList();

	/**
	 @brief 销毁所有待执行的延时任务
	 @return 无
	 */
	void destroyDelayTaskHeap();

private:
	// 保存延时任务的小顶堆
	std::priority_queue<DelalyTask*, std::vector<DelalyTask*>, DelalyTaskCompare> delayTaskHeap_;
	// 空闲的延时任务对象列表
	std::list<DelalyTask*> idleTaskObjList_;
	// 延时任务队列的最大容量
	int maxCapcity_{ 0 };
};
ZCORE_NS_END