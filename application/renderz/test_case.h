#pragma once
#include<iostream>
#include"zthread.h"
#include"zstring.h"
#include"ztime.hpp"
#include"zuuid_utils.h"

class testcase {
public:
	testcase() {
	};
	virtual ~testcase()=default;

	static void threadTest01();//使用封装线程进行任务执行
	static void timeTest01();//时间类的测试
	static void logTest01();//log测试
	static void taskSechTest01();//进行简单的任务调度测试
	static void taskSechTest02();//进行简单的任务调度测试
	static void taskSqliteTest01();//简单数据库测试
	static void taskSqliteTest02();//数据库事务读写
	static void uuidGenrateTest();//uuid生成（暂时用hash函数，碰撞问题之后解决）
	static void jsonReadTest();//读取json配置
};