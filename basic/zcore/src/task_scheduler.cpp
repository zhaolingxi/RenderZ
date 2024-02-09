#include"task_scheduler.h"
ZCORE_NS_BEGIN

TaskScheduler::TaskScheduler(SchedType type, int workerNum)
{
	if (workerNum=-1) {//自动设置线程数量
		if (type == SchedType::WEB_TASK_Sched) {
			threadNum_ = 1;
		}
		else{
			threadNum_ = std::thread::hardware_concurrency();
		}
	}
}

//bool TaskScheduler::singleStep(const uint64_t tickTime, const bool isPollAll)
//{
//
//}

TaskScheduler::SchedType TaskScheduler::getType()
{
	return schedType_;
}

ZCORE_NS_END