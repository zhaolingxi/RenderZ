#include "..\include\task_queue.h"

ZCORE_NS_BEGIN

bool TaskQueue::pushTask(Task*& task, const int& delayTime)
{
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
	if (taskHeap_.empty()) {
		task = nullptr;
		return false;
	}
	task= taskHeap_.top();
	return true;
}
ZCORE_NS_END