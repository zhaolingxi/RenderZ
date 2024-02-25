#include "test_case.h"
#include"ztime.hpp"
#include"task_scheduler.h"
#include "zutils.h"
#include "zcore.h"
#include "log4z.h"
#include"sqlite_operation.h"
#include"zdatabase.h"
#include"zdatabase_operation_define.h"
void func1() {
	std::cout << "testfun1" << std::endl;
}

void func2() {
	std::cout << "testfunction2" << std::endl;
}

void printTimeAndThreadID(int num) {
	auto tid = std::this_thread::get_id();
	int id = *(unsigned int*)&tid;
	//printf("printTimeAndThreadID Number:%d :Time:%d ThreadID:%d \n", num,_getCurrentTime_(), id);

	uint64_t currentTime = _getCurrentTime_();
	zutils::ZString curTime;
	zutils::ZTime::getNowTimeMilliSecStr(curTime);
	LOGFMTI("printTimeAndThreadID Number:%d Time:%d StrTime:%s ThreadID:%d \n", num, _getCurrentTime_(), curTime.getData(), id);
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
	auto it=new zcore::TaskScheduler(zcore::TaskScheduler::SchedType::PRIOR_Sched,-1);
	int num = 3000;
	while (num--) {
		auto func = std::bind(printTimeAndThreadID, num);
		it->postTask(func);
		it->start();
	}
	it->start();
//	it->waitStop();
}

void testcase::taskSechTest02()
{
	auto it = new zcore::TaskScheduler(zcore::TaskScheduler::SchedType::PRIOR_Sched);
	int num = 1000;
	while (num--) {
		auto func = std::bind(printTimeAndThreadID, num);
		it->postTask(func);
		it->start();
	}
	it->waitStop();
}

void testcase::taskSqliteTest01()
{
	zutils::ZString str("testSqliteDB");
	//zdatabase::SQLiteOperation DB(str);
	//zdatabase::SQLiteCmd sqlLine1;
	//sqlLine1.sql_type_ = zdatabase::SqlOperType::SQL_Open;
	//sqlLine1.sqlite_sql_ = "CREATE TABLE IF NOT EXISTS xxx (id integer PRIMARY KEY AUTOINCREMENT, name text NOT NULL, age integer NOT NULL);";
	//DB.excuteSqlOper(sqlLine1);
}

