#include "zsystem_api.h"
#include <ctime>
#include <cstdlib>
#include <thread>
#include <cstring>
#ifdef _MSVC
#include <windows.h>
#include <ShlObj.h>
#include <direct.h>
#else
#include <unistd.h>
#include <pwd.h>
#include <pthread.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#endif
ZUTILS_NS_BEGIN
unsigned long  ZSystemApi::getCurrentThreadId()
{
	unsigned long threadId = 0;
#ifdef _MSVC
	threadId = ::GetCurrentThreadId();
#else
#ifndef SYS_gettid
#define SYS_gettid __NR_gettid
#endif
	// 获取线程ID
	size_t tid = (size_t)syscall(SYS_gettid);
	threadId = (unsigned long)tid;
#endif
	return threadId;
}
int ZSystemApi::getCurrentProcId()
{
	return getpid();
}

void ZSystemApi::getCurrentProcName(ZString& procName)
{
	char name[1024] = { 0 };
#ifdef _MSVC
	::GetModuleFileNameA(nullptr, name, sizeof(name));
#else
	if (readlink("/proc/self/exe", name, sizeof(name) - 1) <= 0) {
		procName = "";
		return;
	}
#endif
	int len = (int)strlen(name);
	int pos = 0;
	for (int i = len - 1; i >= 0; i--) {
		if (name[i] == '\\' || name[i] == '/') {
			pos = i;
			break;
		}
	}
	procName = &name[pos];
}

void ZSystemApi::getCurrentProcDir(ZString& procDir)
{
	char name[1024] = { 0 };
#ifdef _MSVC
	::GetModuleFileNameA(nullptr, name, sizeof(name));
#else
	if (readlink("/proc/self/exe", name, sizeof(name) - 1) <= 0) {
		procDir = "";
		return;
	}
#endif
	int len = (int)strlen(name);
	for (int i = len - 1; i >= 0; i--) {
		if (name[i] == '\\' || name[i] == '/') {
			name[i] = 0;
			break;
		}
	}
	procDir = name;
	procDir.append("/", 1);
}

void ZSystemApi::setWorkingDir(const char* workingDir)
{
#ifdef _MSVC
	(void)_chdir(workingDir);
#else
	chdir(workingDir);
#endif
}

void ZSystemApi::getDocumentDir(ZString& docDir)
{
#ifdef _MSVC
	docDir.ensureBuffer(1024);
	BOOL ret = SHGetSpecialFolderPathA(NULL, (char*)docDir.getData(), CSIDL_MYDOCUMENTS, true);
	if (!ret) {
		docDir = "";
	}
	size_t len = strlen(docDir.getData());
	docDir.setLength(len);
#else
	// linux获取home目录
	ZSystemApi::getUserHomeDir(docDir);
	docDir += "Documents";
#endif
	if (docDir.getLength() == 0) {
#ifdef _MSVC
		docDir.append(".\\", 2); // 默认为当前目录
#else
		docDir.append("./", 2); // 默认为当前目录
#endif
	}
	else {
		if (!docDir.endWith('/') && !docDir.endWith('\\')) {
#ifdef _MSVC
			docDir.append("\\", 1);
#else
			docDir.append("/", 1);
#endif
		}
	}
}

void ZSystemApi::getUserHomeDir(ZString& userHomeDir)
{
#ifdef _MSVC
	userHomeDir.ensureBuffer(1024);
	// windows获取AppData目录
	BOOL ret = SHGetSpecialFolderPathA(NULL, (char*)userHomeDir.getData(), CSIDL_APPDATA, true);
	if (!ret) {
		userHomeDir = "";
	}
	size_t len = strlen(userHomeDir.getData());
	userHomeDir.setLength(len);
#else
	// linux获取home目录
	struct passwd* pw = getpwuid(getuid());
	if (pw == nullptr) {
		userHomeDir = "";
		return;
	}
	userHomeDir = "/home/";
	userHomeDir.append(pw->pw_name);
#endif
	if (userHomeDir.getLength() == 0) {
		userHomeDir.append("./", 2); // 默认为当前目录
	}
	else {
		if (!userHomeDir.endWith('/') && !userHomeDir.endWith('\\')) {
			userHomeDir.append("/", 1);
		}
	}
}

// 产生一个位于 [from, to]的整数，包括from和to，也可以是负数
int ZSystemApi::random(const int from, const int to)
{
	static bool hasSeed = false;
	if (!hasSeed) {
		srand((unsigned int)time(nullptr));
		hasSeed = true;
	}
	int num = rand() % (to - from + 1) + from;
	return num;
}

unsigned int ZSystemApi::getCpuCnt()
{
	unsigned int nthreads = std::thread::hardware_concurrency();
	return nthreads;
}
ZUTILS_NS_END