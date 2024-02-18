#include"task_scheduler.h"
#include <functional>
ZCORE_NS_BEGIN

TaskScheduler::TaskScheduler(SchedType type, int workerNum)
{
	schedThread_ = std::make_shared<zutils::ZThread>("schedThread");
	std::function<void()> it = std::bind(&TaskScheduler::schedule, this);
	schedThread_->setMainTask(it);
	if (workerNum==-1) {//自动设置线程数量
		if (type == SchedType::WEB_TASK_Sched) {
			threadNum_ = 1;
		}
		else{
			threadNum_ = std::thread::hardware_concurrency();
		}
	}
	else {
		threadNum_ = workerNum;
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
	while (taskQueue_ && !taskQueue_->isEmpty()) {
		if (threadQueue_.empty()) {
			if (!threadBusyQueue_.empty()) {
				for (auto it= threadBusyQueue_.begin();it!= threadBusyQueue_.end();)
				{
					if (!(*it)->isThreadRunning()) {
						threadQueue_.push_back(*it);
						it=threadBusyQueue_.erase(it);
						break;
					}
					it++;
				}
				std::chrono::milliseconds duration(singleStepTime_);//所有线程都忙的时候
				std::this_thread::sleep_for(duration);
			}
			continue;
		}
		auto th = threadQueue_.front();
		threadQueue_.pop_front();
		threadBusyQueue_.push_back(th);
		th->setMainTask(taskQueue_->popTask()->taskFunc);
		th->runThread();		
	}
	//任务提交完毕，等待执行完成
	while (!threadBusyQueue_.empty()) {
		for (auto&& it: threadBusyQueue_)
		{
			if (it->isThreadRunning()) {			
				std::chrono::milliseconds duration(maxWaitingTime_);
				std::this_thread::sleep_for(duration);
			}
			threadQueue_.push_back(it);
			threadBusyQueue_.pop_front();
		}
	}
}

void TaskScheduler::start() {
	schedThread_->runThread();
}

void TaskScheduler::pause()
{
	schedThread_->pauseThread();
}

void TaskScheduler::stopNow()
{
	schedThread_->killThread();
}

void TaskScheduler::waitStop()
{
	schedThread_->exitThread();
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