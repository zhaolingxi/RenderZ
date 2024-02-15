#include"task_scheduler.h"
#include <functional>
ZCORE_NS_BEGIN

TaskScheduler::TaskScheduler(SchedType type, int workerNum)
{
	schedThread_ = std::make_shared<zutils::ZThread>("schedThread");
	std::function<void()> it = std::bind(&TaskScheduler::schedule, this);
	schedThread_->setMainTask(it);
	if (workerNum=-1) {//自动设置线程数量
		if (type == SchedType::WEB_TASK_Sched) {
			threadNum_ = 1;
		}
		else{
			threadNum_ = std::thread::hardware_concurrency();
		}
	}
	//数据成员
	//创建线程队列
	int i = 0;
	while (i< threadNum_) {
		auto thread =new zutils::ZThread("TaskSchedulerThread");
		threadQueue_.push_back(thread);
		++i;
	}
	taskQueue_ = std::make_shared<TaskQueue>();
}

void TaskScheduler::schedule()
{
	while (!taskQueue_->isEmpty()) {
		if (threadQueue_.empty()) {
			if (!threadBusyQueue_.front()->isThreadRunning()) {
				threadQueue_.push_back(threadBusyQueue_.front());
				threadBusyQueue_.pop_front();
			}
			continue;
		}
		auto th = threadQueue_.front();
		th->setMainTask(taskQueue_->popTask()->taskFunc);
		th->runThread();
		threadQueue_.pop_front();
		threadBusyQueue_.push_back(th);
	}
}

void TaskScheduler::start() {
	schedThread_->runThread();
}


TaskScheduler::SchedType TaskScheduler::getType()
{
	return schedType_;
}

bool TaskScheduler::postTask(std::function<void()> asyncCallback)
{
	if (!asyncCallback) { return false; }
	return postDelayTask(asyncCallback,0);
}

bool TaskScheduler::postDelayTask(std::function<void()> asyncCallback, const int delayTimeMs)
{
	TaskQueue::Task* task = new TaskQueue::Task();
	task->taskFunc = asyncCallback;
	taskQueue_->pushTask(task, delayTimeMs);
	return false;
}

ZCORE_NS_END