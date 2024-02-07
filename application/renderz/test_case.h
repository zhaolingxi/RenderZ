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

	static void threadTest01();
	static void timeTest01();
};