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

	static void threadTest01();//ʹ�÷�װ�߳̽�������ִ��
	static void timeTest01();//ʱ����Ĳ���
	static void logTest01();//log����
	static void taskSechTest01();//���м򵥵�������Ȳ���
};