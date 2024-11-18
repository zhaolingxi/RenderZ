#include "../include/task_queue.h"

ZCORE_NS_BEGIN

bool TaskQueue::pushTask(Task*& task, const int& delayTime)
{
	std::unique_lock pushLock(lock_);
	if (taskHeap_.size() >= maxCapcity_) {
		return false;
	}

	if (calculateTime(task, delayTime)) {
		taskHeap_.push(task);
		return true;
	}
	return false;
}

TaskQueue::Task* TaskQueue::popTask()
{
	std::unique_lock popLock(lock_);
	if (taskHeap_.empty()) {
		return nullptr;
	}
	auto ret= taskHeap_.top();
	if (ret->tickTime <= _getCurrentTime_()) {
		taskHeap_.pop();
	}
	else {
		return nullptr;
	}
	return ret;
}

bool TaskQueue::topTask(Task*& task)
{
	std::unique_lock topLock(lock_);
	if (taskHeap_.empty()) {
		task = nullptr;
		return false;
	}
	task= taskHeap_.top();
	return true;
}

bool TaskQueue::isEmpty()
{
	return taskHeap_.empty();
}

void TaskQueue::clearTask()
{
	std::unique_lock clearLock(lock_);
	std::priority_queue<Task*, std::vector<Task*>, TaskCompare>  null_queue;
	taskHeap_.swap(null_queue);
}
ZCORE_NS_END