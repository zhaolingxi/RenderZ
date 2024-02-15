#include "test_case.h"
#include"ztime.hpp"
#include"task_scheduler.h"
void func1() {
	std::cout << "testfun1" << std::endl;
}

void func2() {
	std::cout << "testfunction2" << std::endl;
}

void printTimeAndThreadID(int num) {
	auto tid = std::this_thread::get_id();
	int id = *(unsigned int*)&tid;
	printf("printTimeAndThreadID Number:%d :Time:%d ThreadID:%d \n", num,_getCurrentTime_(), id);
}


void testcase::threadTest01()
{
	zutils::ZThread thread1("aa", func1, zutils::runType::LOOP);
	thread1.runThread(zutils::runType::LOOP);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	thread1.pauseThread();

	thread1.exitThread();
//	thread1.killThread();

	thread1.setMainTask(func2);
	thread1.runThread(zutils::runType::ONCE);
}

void testcase::timeTest01()
{
	uint64_t currentTime = _getCurrentTime_();
	zutils::ZString curTime;
	zutils::ZTime::getNowTimeMilliSecStr(curTime);
}

void testcase::taskSechTest01()
{
	auto it=new zcore::TaskScheduler(zcore::TaskScheduler::SchedType::PRIOR_Sched);
	int num = 100;
	while (num--) {
		auto func = std::bind(printTimeAndThreadID, num);
		it->postTask(func);
	}
	it->schedule();
}

