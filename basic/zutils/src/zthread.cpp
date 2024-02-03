#include"zthread.h"
ZUTILS_NS_BEGIN

STDTHREAD ZThread::pstdThread_;
ZThread::ZThread(const char* strName, ZThread* parent, THREAD_MAIN_TASK mainProc)
:threadName_(strName)
{
	parent_ = parent;
}

ZThread::ZThread(ZString strName, ZThread* parent, THREAD_MAIN_TASK mainProc)
:threadName_(strName)
{
	parent_ = parent;
}

ZThread::~ZThread()
{
	if (pstdThread_.joinable()) {
		pstdThread_.join();
	}
}

std::shared_ptr<STDTHREAD> ZThread::getStdThread()
{
	return std::shared_ptr<STDTHREAD>();
}

unsigned int ZThread::getThreadId()
{
	if (threadIdle_) { threadId_ = 0; }
	if (!threadId_) {
		auto tid = std::this_thread::get_id();
		//size_t thread_hash = std::hash<std::thread::id>{}(tid);
		//threadId_ = static_cast<unsigned int>(thread_hash);
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

bool ZThread::runThread(runType iType)
{
	if (!mainTask_) {
		return false;
	}
	if (iType== runType::BLOCK) {
		if (!pstdThread_.native_handle()) {
			pstdThread_ = std::thread(mainTask_);
		}
		threadIdle_.store(false);
		pstdThread_.join();
		threadIdle_.store(true);
	}
	else if (iType == runType::SHARE) {

	}
	
	return true;
}

ZUTILS_NS_END