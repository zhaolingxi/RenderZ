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

	static void threadTest01();//ʹ�÷�װ�߳̽�������ִ��
	static void timeTest01();//ʱ����Ĳ���
	static void logTest01();//log����
	static void taskSechTest01();//���м򵥵�������Ȳ���
	static void taskSechTest02();//���м򵥵�������Ȳ���
	static void taskSqliteTest01();//�����ݿ����
	static void taskSqliteTest02();//���ݿ������д
	static void uuidGenrateTest();//uuid���ɣ���ʱ��hash��������ײ����֮������
	static void jsonReadTest();//��ȡjson����
};