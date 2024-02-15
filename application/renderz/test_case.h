#pragma once
#include<iostream>
#include"zthread.h"
#include"zstring.h"
#include"ztime.hpp"

class testcase {
public:
	testcase() {
	};
	virtual ~testcase()=default;

	static void threadTest01();//使用封装线程进行任务执行
	static void timeTest01();//时间类的测试
	static void logTest01();//log测试
	static void taskSechTest01();//进行简单的任务调度测试
};