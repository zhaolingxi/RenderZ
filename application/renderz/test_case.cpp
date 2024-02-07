#include "test_case.h"
void func1() {
	std::cout << "testfun1" << std::endl;
}

void func2() {
	std::cout << "testfunction2" << std::endl;
}

testcase::~testcase()
{
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

