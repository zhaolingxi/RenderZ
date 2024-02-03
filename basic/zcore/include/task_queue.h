#pragma once
#include "zcore_sdk.h"
#include <queue>
#include <vector>
#include <list>
#include <functional>
/**********************************************************************
 *@brief 任务优先队列，优先级高的先执行
 ***********************************************************************/
ZCORE_NS_BEGIN
using TaskFunc = std::function<void(const int errCode)>;
enum PriorityLevel {Highest=0,Normal,UnImportant};

class ZCORE_API TaskQueue
{
public:
	// 任务
	struct Task
	{
		TaskFunc taskFunc; 
		uint64_t tickTime;   // 延时任务需要执行的时刻
		PriorityLevel priorityLevel;//任务优先级
	};
	// 任务比较函数
	struct TaskCompare
	{
		bool operator()(const Task* left, const Task* right) {
			if (left->priorityLevel!= right->priorityLevel) {
				return left->priorityLevel > right->priorityLevel;
			}
			return left->tickTime > right->tickTime;
		}
	};

	inline calculateTime(Task*& iTask) {

	}

public:
	TaskQueue();
	virtual ~TaskQueue();

private:
	// 保存延时任务的小顶堆
	std::priority_queue<Task*, TaskCompare> delayTaskHeap_;
	// 延时任务队列的最大容量
	int maxCapcity_{ 10000 };
};
ZCORE_NS_END