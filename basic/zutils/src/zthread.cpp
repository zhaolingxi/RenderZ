#include"zthread.h"
#include<iostream>
ZUTILS_NS_BEGIN

ZThread::ZThread(const char* strName, THREAD_MAIN_TASK mainProc, runType type)
:threadName_(strName),
runType_(type)
{
	if (strName) {
		setName(strName);
	}
	if (mainProc) {
		setMainTask(mainProc);
	}
}

ZThread::ZThread(ZString strName, THREAD_MAIN_TASK mainProc, runType type)
:threadName_(strName),
runType_(type)
{
	if (!strName.isEmpty()) {
		setName(strName);
	}
	if (mainProc) {
		setMainTask(mainProc);
	}
}

ZThread::~ZThread()
{
	if (pstdThread_->joinable()) {
		pstdThread_->join();
	}
}

std::shared_ptr<STDTHREAD> ZThread::getStdThread()
{
	return pstdThread_;
}

unsigned int ZThread::getThreadId()
{
	if (threadIdle_) { threadId_ = 0; }
	if (!threadId_) {
		auto tid = std::this_thread::get_id();
		threadId_ = *(unsigned int*)&tid;
	}
	return threadId_;
}

bool ZThread::isThreadIdle()
{
	return threadIdle_.load();
}

bool ZThread::setMainTask(THREAD_MAIN_TASK mainProc)
{
	mainTask_ = mainProc;
	return true;
}

bool ZThread::runThread(runType type)
{
	if (!mainTask_) {
		return false;
	}
	runType_ = type;
	if (pstdThread_ == nullptr /*|| pstdThread_->native_handle()*/ ) {
		std::unique_lock<std::mutex> thread_locker(lock_);
		{
			pstdThread_ = std::make_shared<std::thread>(&ZThread::run, this);
			isRunning_.store(true);//�̱߳�����������std::stread�Ļ��ƣ�������ȴ�ʱ��Ƭִ�У�
		}
		thread_locker.unlock();
		return true;
	}
	if (!isRunning_.load()) {//��ǰ����ͣ״̬�����������߳�
		resumeThread();
	}
	return true;
}

bool ZThread::run()
{
	threadIdle_.store(false);//��������־λ��Ϊbusy
	while (isRunning_.load()) {
		std::cout << std::this_thread::get_id() << std::endl;

		if (runType_ == runType::ONCE) {
			mainTask_();
			isRunning_.store(false);//ִ��һ�κ��߳��Զ�ֹͣ
			mainTask_ = nullptr;
		}
		else if (runType_ == runType::LOOP) {//������ֹͣ��־��ѭ��ִ��
			mainTask_();
		}

		// �߳�ִ����ϻ��ߵ�����ͣ������
		if (!isRunning_.load())
		{
			std::unique_lock<std::mutex> thread_locker(lock_);
			if (!isRunning_.load())
			{
				// �ȴ�������
				task_cv_.wait(thread_locker);
			}
			thread_locker.unlock();
		}
	}

	threadIdle_.store(true);//�������߳̿���
	return true;
}

bool ZThread::pauseThread()
{
	if (pstdThread_) {
		isRunning_.store(false);
		return true;
	}
	return false;
}

bool ZThread::resumeThread()
{
	if (pstdThread_)
	{
		if (!isRunning_.load())//�߳��Ѿ�������ʱ��������
		{
			task_cv_.notify_all();
			isRunning_.store(true);
		}
		return true;
	}
	return false;
}

bool ZThread::killThread()
{
	bool status = false;
	int err=0;
	if (pstdThread_) {
#ifdef _MSVC
		status = ::TerminateThread(pstdThread_->native_handle(), err);
#else
		pthread_cancel(pstdThread_->native_handle());
		status = (err == 0);
#endif
	}
	return status;
}

bool ZThread::exitThread()
{
	if (!pstdThread_ )
	{
		isRunning_.store(false);
		threadIdle_.store(true);
		task_cv_.notify_all();

		if (pstdThread_->joinable()){
			pstdThread_->join();//�ȴ�ԭ���̵߳�����ִ�����
		}
		pstdThread_.reset();//ɾ��ָ����߳�
		return true;
	}
	return false;
}

void ZThread::setName(const char* strName)
{
	threadName_ = strName;
}

void ZThread::setName(const ZString strName)
{
	threadName_ = strName;
}

void ZThread::getName(const char*& strName)
{
	strName = threadName_.getData();
}

void ZThread::getName(ZString& strName)
{
	strName = threadName_;
}

ZUTILS_NS_END