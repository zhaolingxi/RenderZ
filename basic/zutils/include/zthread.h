#pragma once
#include "zutils_sdk.h"
#include<thread>
#include<mutex>
#include <functional>
#include <memory>
#include<atomic>
#include <future>
#include"zstring.h"

/**********************************************************************
 *@brief  线程封装类
 * 核心源自C++ thread
 * 为了减少开销，zthread仅在执行run之后才创建线程实体
 ***********************************************************************/
ZUTILS_NS_BEGIN

#define STDTHREAD std::thread
#define STDFUTURE std::future
#define THREAD_MAIN_TASK std::function<void()>

enum runType { BLOCK=0,SHARE,WAITCB,WAITNOTIFY};

class ZUTILS_API ZThread
{
public:
	explicit ZThread(const char* strName, ZThread* parent = nullptr, THREAD_MAIN_TASK mainProc = nullptr);
	explicit ZThread(ZString strName,ZThread* parent=nullptr, THREAD_MAIN_TASK mainProc=nullptr);
	virtual~ZThread();
	std::shared_ptr<STDTHREAD> getStdThread();

	bool setMainTask(THREAD_MAIN_TASK mainProc);
	bool runThread(runType iType);
	bool killThread();
	bool exitThread();

	unsigned int getThreadId();
	bool isThreadIdle();

	void setName(const char* strName);
	void setName(const ZString strName);
	void getName(const char*& strName);
	void getName(const ZString& strName);

private:
	static STDTHREAD pstdThread_;
	ZString threadName_;
	THREAD_MAIN_TASK mainTask_{ nullptr };
	unsigned int threadId_{ 0 };
	ZThread* parent_{ nullptr };
	std::atomic<bool> threadIdle_{ true };
	std::condition_variable task_cv_;
	std::mutex lock_;
};
ZUTILS_NS_END