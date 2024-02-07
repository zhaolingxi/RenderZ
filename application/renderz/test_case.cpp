#include "test_case.h"
#include"ztime.hpp"
void func1() {
	std::cout << "testfun1" << std::endl;
}

void func2() {
	std::cout << "testfunction2" << std::endl;
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

