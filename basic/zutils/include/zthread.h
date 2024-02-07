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
 *@brief  �̷߳�װ��
 * author:zhaolingxi
 * ����Դ��C++ thread
 * Ϊ�˼��ٿ�����zthread����ִ��run֮��Ŵ����߳�ʵ��
 ***********************************************************************/
ZUTILS_NS_BEGIN

#define STDTHREAD std::thread
#define STDFUTURE std::future
#define THREAD_MAIN_TASK std::function<void()>

enum runType {ONCE=0,LOOP};

class ZUTILS_API ZThread
{
public:
	explicit ZThread(const char* strName, THREAD_MAIN_TASK mainProc = nullptr,runType type = runType::ONCE);
	explicit ZThread(ZString strName,THREAD_MAIN_TASK mainProc=nullptr, runType type = runType::ONCE);
	virtual~ZThread();
	std::shared_ptr<STDTHREAD> getStdThread();

	bool setMainTask(THREAD_MAIN_TASK mainProc);
	bool runThread(runType type= runType::ONCE);//�����������߳�ʵ��
	bool pauseThread();//��ͣ�̣߳��ȴ��´�ִ��
	bool resumeThread();//�����̣߳�ִ���߳�������
	bool killThread();//ǿ���˳��̣߳�ֱ���˳���
	bool exitThread();//�˳��߳�(�ȴ�ִ�����һ������)��ԭ���̻߳ᱻɾ��

	unsigned int getThreadId();
	bool isThreadIdle();

	//template <typename T>
	void setName(const char* strName);
	void setName(const ZString strName);
	void getName(const char*& strName);
	void getName(ZString& strName);

private:
	bool run();


	std::shared_ptr <STDTHREAD> pstdThread_;
	ZString threadName_;
	THREAD_MAIN_TASK mainTask_{ nullptr };
	unsigned int threadId_{ 0 };
	std::atomic<bool> threadIdle_{ true };//�߳��Ƿ��ڿ���״̬������״̬����æ�ȣ�
	std::atomic<bool> isRunning_{ false };//�߳��Ƿ�������״̬������״̬���Ƿ�����޹أ�
	std::condition_variable task_cv_;
	std::mutex lock_;
	runType runType_{ ONCE };
};
ZUTILS_NS_END