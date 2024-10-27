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
 * author:zhaolingxi
 * 核心源自C++ thread
 * 为了减少开销，zthread仅在执行run之后才创建线程实体
 ***********************************************************************/
ZUTILS_NS_BEGIN

#define STDTHREAD std::thread
#define STDFUTURE std::future
#define THREAD_MAIN_TASK std::function<void()>

enum runType {ONCE=0,LOOP};

class ZUTILS_API ZThread
{
	struct ZThreadCompare
	{
		bool operator()(const ZThread* left, const ZThread* right) {
			if (left->isRunning_) {//空闲优先
				return false;
			}
			else if (right->isRunning_) {
				return false;
			}
			else {//比较id
				return (left->threadId_ < right->threadId_);
			}
		}
	};

public:
	explicit ZThread(const char* strName, THREAD_MAIN_TASK mainProc = nullptr,runType type = runType::ONCE);
	explicit ZThread(ZString strName,THREAD_MAIN_TASK mainProc=nullptr, runType type = runType::ONCE);

	void operator=(const ZThread& other) = delete;
	void operator=(const ZThread&& other) = delete;


	virtual~ZThread();
	std::shared_ptr<STDTHREAD> getStdThread();

	bool setMainTask(THREAD_MAIN_TASK mainProc);
	bool runThread(runType type= runType::ONCE);//创建或运行线程实体
	bool pauseThread();//暂停线程，等待下次执行
	bool resumeThread();//重启线程，执行线程主函数
	bool killThread();//强制退出线程（直接退出）
	bool exitThread();//退出线程(等待执行最后一次任务)，原有线程会被删除

	unsigned int getThreadId();
	bool isThreadIdle();
	bool isThreadRunning();

	//template <typename T>
	void setName(const char* strName);
	void setName(const ZString strName);
	void getName(const char*& strName);
	void getName(ZString& strName);

	unsigned int threadId_{ 0 };

private:
	bool run();
	std::shared_ptr <STDTHREAD> pstdThread_;
	ZString threadName_;
	THREAD_MAIN_TASK mainTask_{ nullptr };
	std::atomic<bool> isRunning_{ false };//线程是否处于运行状态（运行状态与是否空闲无关）
	std::condition_variable task_cv_;
	std::mutex lock_;
	std::atomic <runType> runType_{ ONCE };
};
ZUTILS_NS_END