/*
 * Log4z License
 * -----------
 *
 * Log4z is licensed under the terms of the MIT license reproduced below.
 * This means that Log4z is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 *
 *
 * ===============================================================================
 *
 * Copyright (C) 2010-2013 YaweiZhang <yawei_zhang@foxmail.com>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ===============================================================================
 *
 * (end of COPYRIGHT)
 */
#include "log4z.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>


#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma warning(disable:4996)

#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#endif

 // the macor for mac
#ifdef _APPLE_
#include <dispatch/dispatch.h>
#include <libproc.h>
#endif

_ZSUMMER_BEGIN
_ZSUMMER_LOG4Z_BEGIN

static const char *const LOG_STRING[] =
{
	"LOG_TRACE",
	"LOG_DEBUG",
	"LOG_INFO ",
	"LOG_WARN ",
	"LOG_ERROR",
	"LOG_ALARM",
	"LOG_FATAL",
};

// the color of the log text
#if defined(_WIN32) || defined(_WIN64)
const static WORD LOG_COLOR[LOG_LEVEL_FATAL + 1] =
{
	0,
	0,
	FOREGROUND_BLUE | FOREGROUND_GREEN,
	FOREGROUND_GREEN | FOREGROUND_RED,
	FOREGROUND_RED,
	FOREGROUND_GREEN,
	FOREGROUND_RED | FOREGROUND_BLUE
};
#else
const static char LOG_COLOR[LOG_LEVEL_FATAL + 1][50] =
{
	"\e[0m",
	"\e[0m",
	"\e[34m\e[1m",//hight blue
	"\e[33m", //yellow
	"\e[31m", //red
	"\e[32m", //green
	"\e[35m"
};
#endif

//////////////////////////////////////////////////////////////////////////
//! Log4zFileHandler
//////////////////////////////////////////////////////////////////////////
class ZCORE_API Log4zFileHandler
{
public:
	Log4zFileHandler()
	{
		_file = NULL;
		_filePath = "";
	}
	~Log4zFileHandler()
	{
		close();
	}
	inline bool isOpen()
	{
		return _file != NULL;
	}
	inline bool open(const char *path, const char * mod)
	{
		if (_file != NULL)
		{
			fclose(_file);
			_file = NULL;
		}
		_filePath = path;
		_file = fopen(path, mod);
		return _file != NULL;
	}
	inline void close()
	{
		if (_file != NULL)
		{
			fclose(_file);
			_file = NULL;
		}
	}
	inline void write(const char * data, size_t len)
	{
		if (_file && len > 0)
		{
			if (fwrite(data, 1, len, _file) != len)
			{
				close();
			}
		}
	}
	inline void flush()
	{
		if (_file)
		{
			fflush(_file);
		}
	}

	inline unsigned int getSize()
	{
		if (_file)
		{
			fseek(_file, 0, SEEK_END);
			return ftell(_file);
		}

		return 0;
	}

	inline std::string readLine()
	{
		char buf[500] = { 0 };

		if (_file && fgets(buf, 500, _file) != NULL)
		{
			return std::string(buf);
		}

		return std::string("");
	}
	inline const std::string readContent();

	inline const std::string &getFilePath() const
	{
		return _filePath;
	}

	inline const std::string &getBakPathPrefix() const
	{
		return _bakPathPrefix;
	}

	inline void setBakPathPrefix(const std::string &pathPrefix)
	{
		_bakPathPrefix = pathPrefix;
	}
public:
	FILE *_file{ nullptr };
	std::string _filePath{ "" };
	std::string _bakPathPrefix{ "" };
};


//////////////////////////////////////////////////////////////////////////
//! UTILITY
//////////////////////////////////////////////////////////////////////////
static void sleepMillisecond(unsigned int ms);
static tm timeToTm(time_t t);
static bool isSameDay(time_t t1, time_t t2);

static void fixPath(std::string &path);
static void trimLogConfig(std::string &str, std::string extIgnore = std::string());
static std::pair<std::string, std::string> splitPairString(const std::string & str, const std::string & delimiter);


static bool isDirectory(std::string path);
static bool createRecursionDir(std::string path);
static std::string getProcessID();
static std::string getProcessName();



//////////////////////////////////////////////////////////////////////////
//! LockHelper
//////////////////////////////////////////////////////////////////////////
class ZCORE_API LockHelper
{
public:
	LockHelper();
	virtual ~LockHelper();

public:
	void lock();
	void unLock();
private:
#if defined(_WIN32) || defined(_WIN64)
	CRITICAL_SECTION _crit;
#else
	pthread_mutex_t  _crit;
#endif
};

//////////////////////////////////////////////////////////////////////////
//! Log4zAutoLocker
//////////////////////////////////////////////////////////////////////////
class ZCORE_API Log4zAutoLocker
{
public:
	explicit Log4zAutoLocker(LockHelper & lk) : _lock(lk)
	{
		_lock.lock();
	}
	~Log4zAutoLocker()
	{
		_lock.unLock();
	}
private:
	LockHelper & _lock;
};

//////////////////////////////////////////////////////////////////////////
//! SemHelper
//////////////////////////////////////////////////////////////////////////
class SemHelper
{
public:
	SemHelper();
	virtual ~SemHelper();
public:
	bool create(int initcount);
	bool wait(int timeout = 0);
	bool post();
private:
#if defined(_WIN32) || defined(_WIN64)
	HANDLE _hSem;
#elif defined(__APPLE__)
	dispatch_semaphore_t _semid;
#else
	sem_t _semid;
	bool  _isCreate;
#endif

};

//////////////////////////////////////////////////////////////////////////
//! ThreadHelper
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(_WIN64)
static unsigned int WINAPI  threadProc(LPVOID lpParam);
#else
static void * threadProc(void * pParam);
#endif

class ThreadHelper
{
public:
	ThreadHelper()
	{
		_hThreadID = 0;
	}
	virtual ~ThreadHelper() {}
public:
	bool start();
	bool wait();
	virtual void run() = 0;
private:
	unsigned long long _hThreadID;
#if !(defined(_WIN32) || defined(_WIN64))
	pthread_t _phtreadID;
#endif
};

#if (defined(_WIN32) || defined(_WIN64))
unsigned int WINAPI  threadProc(LPVOID lpParam)
{
	ThreadHelper * p = (ThreadHelper *)lpParam;
	p->run();
	return 0;
}
#else
void * threadProc(void * pParam)
{
	ThreadHelper * p = (ThreadHelper *)pParam;
	p->run();
	return NULL;
}
#endif


//////////////////////////////////////////////////////////////////////////
//! LogData
//////////////////////////////////////////////////////////////////////////
struct LogData
{
	LoggerId _id;       //dest logger id
	int _level; //log level
	time_t _time;       //create time
	unsigned int _precise; //create time
	int _contentLen;
	char _content[LOG4Z_LOG_BUF_SIZE]; //content
};

//////////////////////////////////////////////////////////////////////////
//! LoggerInfo
//////////////////////////////////////////////////////////////////////////
struct LoggerInfo
{
	// 日志配置
	std::string _key;   //logger key
	std::string _name;  // one logger one name.
	std::string _path;  //path for log file.
	int  _level;        //filter level
	bool _display;      //display to screen
	bool _outfile;      //output to file
	bool _monthdir;     //create directory per month
	unsigned int _limitsize; //limit file's size, unit Million byte.
	bool _enable;       //logger is enable
	bool _fileLine;     //enable/disable the log's suffix.(file name:line number)
	bool _rebootDelete; // 操作系统下次重启时是否删除创建的文件
	int _maxLogFileCnt;  // 最大保留的.log文件的数量
	int _fileClearCycleHour; // 日志文件清理周期(单位：小时)


	// 日志运行时记录
	time_t _loggerStartTime; // 日志Logger的开启时间
	time_t _lastFileClearTime; // 上一次清理日志文件的时间
	time_t _curFileCreateTime;  //file create time
	unsigned int _curWriteLen;  //current file length
	Log4zFileHandler    _handle;        //file handle
	int _needFlushLogCnt; // 需要刷新的日志条数
	bool _hotChange; // 是否需要对日志配置进行热更新

	LoggerInfo()
	{
		// 日志配置
		_enable = false;
		_path = LOG4Z_DEFAULT_PATH;
		_level = LOG4Z_DEFAULT_LEVEL;
		_display = LOG4Z_DEFAULT_DISPLAY;
		_outfile = LOG4Z_DEFAULT_OUTFILE;
		_monthdir = LOG4Z_DEFAULT_MONTHDIR;
		_limitsize = LOG4Z_DEFAULT_LIMITSIZE;
		_fileLine = LOG4Z_DEFAULT_SHOWSUFFIX;
		_rebootDelete = LOG4Z_DEFAULT_REBOOT_DEL;
		_maxLogFileCnt = LOG4Z_DEFAULT_MAX_LOGFILE_CNT;
		_fileClearCycleHour = LOG4Z_DEFAULT_FILE_CLEAR_CYCLE;

		// 日志运行时记录
		_curFileCreateTime = 0;
		_loggerStartTime = 0;
		_curWriteLen = 0;
		_needFlushLogCnt = 0;

		_hotChange = false;
	}
};


//////////////////////////////////////////////////////////////////////////
//! LogerManager
//////////////////////////////////////////////////////////////////////////
class LogerManager : public ThreadHelper, public ILog4zManager
{
public:
	static LogerManager *instance;

public:
	LogerManager();
	virtual ~LogerManager();

public:
	bool configFromStringImpl(std::string content, bool isUpdate);
	//! 读取配置文件并覆盖原有配置
	bool config(const char* configPath) override;
	bool configFromString(const char* configContent) override;

	//! 覆写式创建
	LoggerId createLogger(const char* key) override;
	bool start() override;
	bool stop() override;
	//! 查找ID
	LoggerId findLogger(const char*  key) override;

	bool prePushLog(LoggerId id, int level) override;
	bool pushLog(LoggerId id, int level, const char * log, const char * file, int line) override;

	bool enableLogger(LoggerId id, bool enable) override;
	bool setLoggerName(LoggerId id, const char * name) override;
	bool setLoggerPath(LoggerId id, const char * path) override;
	bool setLoggerLevel(LoggerId id, int nLevel) override;
	bool setLoggerFileLine(LoggerId id, bool enable) override;
	bool setLoggerDisplay(LoggerId id, bool enable) override;
	bool setLoggerOutFile(LoggerId id, bool enable) override;
	bool setLoggerLimitsize(LoggerId id, unsigned int limitsize) override;
	bool setLoggerMonthdir(LoggerId id, bool enable) override;
	bool setLoggerRebootDelete(LoggerId id, bool enable) override;
	bool setLoggerMaxLogFileCnt(LoggerId id, int maxLogFileCnt) override;
	bool setLoggerFileClearCycleHour(LoggerId id, int clearCycleHour) override;

	bool setAutoUpdate(int interval) override;
	bool updateConfig() override;
	bool isLoggerEnable(LoggerId id) override;
	unsigned long long getStatusTotalWriteCount() override
	{
		return _ullStatusTotalWriteFileCount;
	}
	unsigned long long getStatusTotalWriteBytes() override
	{
		return _ullStatusTotalWriteFileBytes;
	}
	unsigned long long getStatusWaitingCount() override
	{
		return _ullStatusTotalPushLog - _ullStatusTotalPopLog;
	}
	unsigned int getStatusActiveLoggers() override;

protected:
	// 更新最后一次清理日志文件的时间
	bool setLoggerLastFileClearTime(LoggerId id, time_t time);
	// 更新logger的启动时间
	bool setLoggerStartTime(LoggerId id, time_t time);
	// 清除多余的日志文件
	void clearExpireLogFiles(const int loggerId);

protected:
	void showColorText(const char *text, int level = LOG_LEVEL_DEBUG);
	bool openLogger(LogData * log);
	bool closeLogger(LoggerId id);
	bool popLog(LogData *& log);
	virtual void run();

private:

	//! thread status.
	bool        _runing;
	//! wait thread started.
	SemHelper       _semaphore;

	//! hot change name or path for one logger
	LockHelper _hotLock;
	int _hotUpdateCfgInterval{ 0 };

	//! the process info.
	std::string _pid;
	std::string _proName;

	//! config file name
	std::string _configFile;

	//! logger id manager, [logger name]:[logger id].
	std::map<std::string, LoggerId> _ids;
	// the last used id of _loggers
	LoggerId    _lastId;
	LoggerInfo _loggers[LOG4Z_LOGGER_MAX];

	//! log queue
	std::list<LogData *> _logs;
	LockHelper  _logLock;

	//show color lock
	LockHelper _scLock;
	//status statistics
	//write file
	unsigned long long _ullStatusTotalWriteFileCount;
	unsigned long long _ullStatusTotalWriteFileBytes;

	//Log queue statistics
	unsigned long long _ullStatusTotalPushLog;
	unsigned long long _ullStatusTotalPopLog;

};




//////////////////////////////////////////////////////////////////////////
//! Log4zFileHandler
//////////////////////////////////////////////////////////////////////////

const std::string Log4zFileHandler::readContent()
{
	std::string content;

	if (!_file)
	{
		return content;
	}

	fseek(_file, 0, SEEK_SET);
	int beginpos = ftell(_file);
	fseek(_file, 0, SEEK_END);
	int endpos = ftell(_file);
	fseek(_file, 0, SEEK_SET);
	int filelen = endpos - beginpos;

	if (filelen > 10 * 1024 * 1024 || filelen <= 0)
	{
		return content;
	}

	content.resize(filelen + 10);

	if (fread(&content[0], 1, filelen, _file) != (size_t)filelen)
	{
		content.clear();
		return content;
	}

	content = content.c_str();
	return content;
}

//////////////////////////////////////////////////////////////////////////
//! utility
//////////////////////////////////////////////////////////////////////////
void sleepMillisecond(unsigned int ms)
{
#if defined(_WIN32) || defined(_WIN64)
	::Sleep(ms);
#else
	usleep(1000 * ms);
#endif
}

struct tm timeToTm(time_t t)
{
#if defined(_WIN32) || defined(_WIN64)
#if _MSC_VER < 1400 //VS2003
	return *localtime(&t);
#else //vs2005->vs2013->
	struct tm tt = { 0 };
	localtime_s(&tt, &t);
	return tt;
#endif
#else //linux
	struct tm tt = { 0 };
	localtime_r(&t, &tt);
	return tt;
#endif
}

bool isSameDay(time_t t1, time_t t2)
{
	tm tm1 = timeToTm(t1);
	tm tm2 = timeToTm(t2);
	return (tm1.tm_year == tm2.tm_year && tm1.tm_yday == tm2.tm_yday);
}

// 补全和替换路径中是 '/'
void fixPath(std::string &path)
{
	if (path.empty()) {
		return;
	}

	for (std::string::iterator iter = path.begin(); iter != path.end(); ++iter) {
		if (*iter == '\\') {
			*iter = '/';
		}
	}

	if (path.at(path.length() - 1) != '/') {
		path.append("/");
	}
}

static void trimLogConfig(std::string &str, std::string extIgnore)
{
	if (str.empty()) {
		return;
	}

	extIgnore += "\r\n\t ";
	int length = (int)str.length();
	int posBegin = 0;
	int posEnd = 0;

	//trim utf8 file bom
	if (str.length() >= 3
		&& (unsigned char)str[0] == 0xef
		&& (unsigned char)str[1] == 0xbb && (unsigned char)str[2] == 0xbf) {
		posBegin = 3;
	}

	//trim character
	for (int i = posBegin; i < length; i++)
	{
		bool bCheck = false;
		for (int j = 0; j < (int)extIgnore.length(); j++) {
			if (str[i] == extIgnore[j]) {
				bCheck = true;
				break;
			}
		}

		if (bCheck) {
			if (i == posBegin) {
				posBegin++;
			}
		}
		else {
			posEnd = i + 1;
		}
	}

	if (posBegin < posEnd) {
		str = str.substr(posBegin, posEnd - posBegin);
	}
	else {
		str.clear();
	}
}

//split
static std::pair<std::string, std::string> splitPairString(const std::string & str, const std::string & delimiter)
{
	std::string::size_type pos = str.find(delimiter.c_str());

	if (pos == std::string::npos) {
		return std::make_pair(str, "");
	}
	return std::make_pair(str.substr(0, pos), str.substr(pos + delimiter.length()));
}

static bool parseConfigLine(const std::string& line, int curLineNum, std::string & key, std::map<std::string, LoggerInfo> & outInfo)
{
	std::pair<std::string, std::string> kv = splitPairString(line, "=");

	if (kv.first.empty()) {
		return false;
	}

	trimLogConfig(kv.first);
	trimLogConfig(kv.second);

	if (kv.first.empty() || kv.first.at(0) == '#') {
		return true;
	}

	if (kv.first.at(0) == '[')
	{
		trimLogConfig(kv.first, "[]");
		key = kv.first;
		{
			std::string tmpstr = kv.first;
			std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), ::tolower);

			if (tmpstr == "main")
			{
				key = "Main";
			}
		}
		std::map<std::string, LoggerInfo>::iterator iter = outInfo.find(key);

		if (iter == outInfo.end())
		{
			LoggerInfo li;
			li._enable = true;
			li._key = key;
			li._name = key;
			outInfo.insert(std::make_pair(li._key, li));
		}
		else
		{
			std::cout << "log4z configure warning: duplicate logger key:[" << key << "] at line:" << curLineNum << std::endl;
		}

		return true;
	}

	trimLogConfig(kv.first);
	trimLogConfig(kv.second);
	std::map<std::string, LoggerInfo>::iterator iter = outInfo.find(key);

	if (iter == outInfo.end())
	{
		std::cout << "log4z configure warning: not found current logger name:[" << key << "] at line:" << curLineNum
			<< ", key=" << kv.first << ", value=" << kv.second << std::endl;
		return true;
	}

	// 将所有配置项的key转换为小写
	std::transform(kv.first.begin(), kv.first.end(), kv.first.begin(), ::tolower);

	//! path
	if (kv.first == "path") {
		std::string path = kv.second;
		int len = path.size();
		if (len > 0 && path[len - 1] != '/' && path[len - 1] != '\\') {
			path += '/';
		}
		iter->second._path = path;
		return true;
	}
	else if (kv.first == "name")
	{
		iter->second._name = kv.second;
		return true;
	}

	std::transform(kv.second.begin(), kv.second.end(), kv.second.begin(), ::tolower);

	//! level
	if (kv.first == "level")
	{
		if (kv.second == "trace" || kv.second == "all")
		{
			iter->second._level = LOG_LEVEL_TRACE;
		}
		else if (kv.second == "debug")
		{
			iter->second._level = LOG_LEVEL_DEBUG;
		}
		else if (kv.second == "info")
		{
			iter->second._level = LOG_LEVEL_INFO;
		}
		else if (kv.second == "warn" || kv.second == "warning")
		{
			iter->second._level = LOG_LEVEL_WARN;
		}
		else if (kv.second == "error")
		{
			iter->second._level = LOG_LEVEL_ERROR;
		}
		else if (kv.second == "alarm")
		{
			iter->second._level = LOG_LEVEL_ALARM;
		}
		else if (kv.second == "fatal")
		{
			iter->second._level = LOG_LEVEL_FATAL;
		}
	}
	//! display
	else if (kv.first == "display")
	{
		iter->second._display = true;
		if (kv.second == "false" || kv.second == "0") {
			iter->second._display = false;
		}
	}
	//! output to file
	else if (kv.first == "outfile")
	{
		iter->second._outfile = true;
		if (kv.second == "false" || kv.second == "0") {
			iter->second._outfile = false;
		}
	}
	//! monthdir
	else if (kv.first == "monthdir")
	{
		iter->second._monthdir = true;
		if (kv.second == "false" || kv.second == "0") {
			iter->second._monthdir = false;
		}
	}
	//! limit file size
	else if (kv.first == "limitsize")
	{
		iter->second._limitsize = atoi(kv.second.c_str());
	}
	//! display log in file line
	else if (kv.first == "fileline")
	{
		iter->second._fileLine = true;
		if (kv.second == "false" || kv.second == "0") {
			iter->second._fileLine = false;
		}
	}
	//! enable/disable one logger
	else if (kv.first == "enable")
	{
		iter->second._enable = true;
		if (kv.second == "false" || kv.second == "0") {
			iter->second._enable = false;
		}
	}
	else if (kv.first == "rebootdel")
	{
		iter->second._rebootDelete = true;
		if (kv.second == "false" || kv.second == "0")
		{
			iter->second._rebootDelete = false;
		}
	}
	else if (kv.first == "maxlogfilecnt") // 最大保留的日志文件数量
	{
		iter->second._maxLogFileCnt = atoi(kv.second.c_str());
		// 默认保留10个最近的.log文件
		if (iter->second._maxLogFileCnt <= 0) {
			iter->second._maxLogFileCnt = LOG4Z_DEFAULT_MAX_LOGFILE_CNT;
		}
	}
	else if (kv.first == "fileclearcycle") // 最大保留的日志文件数量
	{
		iter->second._fileClearCycleHour = atoi(kv.second.c_str());
		// 默认保留10个最近的.log文件
		if (iter->second._fileClearCycleHour <= 0) {
			iter->second._fileClearCycleHour = LOG4Z_DEFAULT_FILE_CLEAR_CYCLE;
		}
	}
	return true;
}

static bool parseConfigFromString(std::string content, std::map<std::string, LoggerInfo> & outInfo)
{
	std::string key;
	int curLine = 1;
	std::string line;
	std::string::size_type curPos = 0;
	if (content.empty()) {
		return true;
	}

	while (1) {
		std::string::size_type pos = std::string::npos;

		for (std::string::size_type i = curPos; i < content.length(); ++i)
		{
			//support linux/unix/windows LRCF
			if (content[i] == '\r' || content[i] == '\n')
			{
				pos = i;
				break;
			}
		}
		line = content.substr(curPos, pos - curPos);
		parseConfigLine(line, curLine, key, outInfo);
		curLine++;
		if (pos == std::string::npos) {
			break;
		}
		else {
			curPos = pos + 1;
		}
	}
	return true;
}

bool isDirectory(std::string path)
{
#if defined(_WIN32) || defined(_WIN64)
	return ::PathIsDirectoryA(path.c_str()) ? true : false;
#else
	DIR * pdir = opendir(path.c_str());
	if (pdir == NULL) {
		return false;
	}
	closedir(pdir);
	pdir = NULL;
	return true;
#endif
}

bool createRecursionDir(std::string path)
{
	if (path.length() == 0) {
		return true;
	}

	std::string sub("");
	fixPath(path);

	bool ret = true;
	std::string::size_type pos = path.find('/');
	if (pos == std::string::npos) {
		pos = path.find('\\');
	}
	while (pos != std::string::npos) {
		std::string cur = path.substr(0, pos);
		if (cur.length() > 0 && !isDirectory(cur)) {
#if defined(_WIN32) || defined(_WIN64)
			ret = ::CreateDirectoryA(cur.c_str(), NULL) ? true : false;
#else
			ret = (mkdir(cur.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0);
#endif
			if (!ret) {
				break;
			}
		}
		pos = path.find('/', pos + 1);
		if (pos == std::string::npos) {
			pos = path.find('\\', pos + 1);
		}
	}
	return ret;
}

std::string getProcessID()
{
	std::string pid = "0";
	char buf[260] = { 0 };
#if defined(_WIN32) || defined(_WIN64)
	DWORD winPID = GetCurrentProcessId();
	sprintf(buf, "%06d", winPID);
	pid = buf;
#else
	sprintf(buf, "%06d", getpid());
	pid = buf;
#endif
	return pid;
}

std::string getProcessName()
{
	std::string name = "MainLog";
	char buf[260] = { 0 };
#if defined(_WIN32) || defined(_WIN64)
	if (::GetModuleFileNameA(NULL, buf, 259) > 0) {
		name = buf;
	}

	std::string::size_type pos = name.rfind("\\");
	if (pos != std::string::npos) {
		name = name.substr(pos + 1, std::string::npos);
	}
	pos = name.rfind(".");
	if (pos != std::string::npos) {
		name = name.substr(0, pos - 0);
	}

#elif defined(__APPLE__)
	proc_name(getpid(), buf, 260);
	name = buf;
	return name;;
#else
	sprintf(buf, "/proc/%d/cmdline", (int)getpid());
	Log4zFileHandler i;
	i.open(buf, "r");
	if (!i.isOpen()) {
		return name;
	}
	name = i.readLine();
	i.close();
	std::string::size_type pos = name.rfind("/");
	if (pos != std::string::npos) {
		name = name.substr(pos + 1, std::string::npos);
	}
#endif
	return name;
}

//////////////////////////////////////////////////////////////////////////
// LockHelper
//////////////////////////////////////////////////////////////////////////
LockHelper::LockHelper()
{
#if defined(_WIN32) || defined(_WIN64)
	::InitializeCriticalSection(&_crit);
#else
	//_crit = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&_crit, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}
LockHelper::~LockHelper()
{
#if defined(_WIN32) || defined(_WIN64)
	::DeleteCriticalSection(&_crit);
#else
	pthread_mutex_destroy(&_crit);
#endif
}

void LockHelper::lock()
{
#if defined(_WIN32) || defined(_WIN64)
	::EnterCriticalSection(&_crit);
#else
	pthread_mutex_lock(&_crit);
#endif
}
void LockHelper::unLock()
{
#if defined(_WIN32) || defined(_WIN64)
	::LeaveCriticalSection(&_crit);
#else
	pthread_mutex_unlock(&_crit);
#endif
}
//////////////////////////////////////////////////////////////////////////
// SemHelper
//////////////////////////////////////////////////////////////////////////
SemHelper::SemHelper()
{
#if defined(_WIN32) || defined(_WIN64)
	_hSem = NULL;
#elif defined(__APPLE__)
	_semid = NULL;
#else
	_isCreate = false;
#endif

}
SemHelper::~SemHelper()
{
#if defined(_WIN32) || defined(_WIN64)
	if (_hSem != NULL) {
		CloseHandle(_hSem);
		_hSem = NULL;
	}
#elif defined(__APPLE__)
	if (_semid) {
		dispatch_release(_semid);
		_semid = NULL;
	}
#else
	if (_isCreate) {
		_isCreate = false;
		sem_destroy(&_semid);
	}
#endif
}

bool SemHelper::create(int initcount)
{
	if (initcount < 0) {
		initcount = 0;
	}
#if defined(_WIN32) || defined(_WIN64)
	if (initcount > 64) {
		return false;
	}
	_hSem = ::CreateSemaphore(NULL, initcount, 64, NULL);
	if (_hSem == NULL) {
		return false;
	}
#elif defined(__APPLE__)
	_semid = dispatch_semaphore_create(initcount);
	if (!_semid) {
		return false;
	}
#else
	if (sem_init(&_semid, 0, initcount) != 0) {
		return false;
	}
	_isCreate = true;
#endif
	return true;
}
bool SemHelper::wait(int timeout)
{
#if defined(_WIN32) || defined(_WIN64)
	if (timeout < 0) {
		timeout = INFINITE;
	}
	if (::WaitForSingleObject(_hSem, timeout) != WAIT_OBJECT_0) {
		return false;
	}
#elif defined(__APPLE__)

	if (dispatch_semaphore_wait(_semid, dispatch_time(DISPATCH_TIME_NOW, timeout * 1000)) != 0) {
		return false;
	}
#else
	if (timeout < 0) {
		return (sem_wait(&_semid) == 0);
	}

	struct timeval tm;
	gettimeofday(&tm, NULL);
	long long endtime = tm.tv_sec * 1000 + tm.tv_usec / 1000 + timeout;
	bool status = false;
	while (1) {
		sleepMillisecond(50);
		int ret = sem_trywait(&_semid);
		if (ret == 0) {
			status = true;
			break;
		}

		struct timeval tv_cur;
		gettimeofday(&tv_cur, NULL);
		// wait timeout
		if (tv_cur.tv_sec * 1000 + tv_cur.tv_usec / 1000 > endtime) {
			status = false;
			break;
		}
		if (ret == -1 && errno == EAGAIN) {
			continue;
		}
		else {
			status = false;
			break;
		}
	}
	return status;
#endif
	return true;
}

bool SemHelper::post()
{
#if defined(_WIN32) || defined(_WIN64)
	return ::ReleaseSemaphore(_hSem, 1, NULL) ? true : false;
#elif defined(__APPLE__)
	return dispatch_semaphore_signal(_semid) == 0;
#else
	return (sem_post(&_semid) == 0);
#endif
}

//////////////////////////////////////////////////////////////////////////
//! ThreadHelper
//////////////////////////////////////////////////////////////////////////
bool ThreadHelper::start()
{
#if defined(_WIN32) || defined(_WIN64)
	unsigned long long ret = _beginthreadex(NULL, 0, threadProc, (void *)this, 0, NULL);
	if (ret == -1 || ret == 0) {
		std::cout << "log4z: create log4z thread error! \r\n" << std::endl;
		return false;
	}
	_hThreadID = ret;
#else
	int ret = pthread_create(&_phtreadID, NULL, threadProc, (void*)this);
	if (ret != 0) {
		std::cout << "log4z: create log4z thread error! \r\n" << std::endl;
		return false;
	}
#endif
	return true;
}

bool ThreadHelper::wait()
{
#if defined(_WIN32) || defined(_WIN64)
	if (::WaitForSingleObject((HANDLE)_hThreadID, INFINITE) != WAIT_OBJECT_0) {
		return false;
	}
#else
	if (pthread_join(_phtreadID, NULL) != 0) {
		return false;
	}
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////
//! LogerManager
//////////////////////////////////////////////////////////////////////////
LogerManager::LogerManager()
{
	_runing = false;
	_lastId = LOG4Z_MAIN_LOGGER_ID;
	_hotUpdateCfgInterval = 0;

	_ullStatusTotalPushLog = 0;
	_ullStatusTotalPopLog = 0;
	_ullStatusTotalWriteFileCount = 0;
	_ullStatusTotalWriteFileBytes = 0;

	_pid = getProcessID();
	_proName = getProcessName();
	_loggers[LOG4Z_MAIN_LOGGER_ID]._enable = true;
	_ids[LOG4Z_MAIN_LOGGER_KEY] = LOG4Z_MAIN_LOGGER_ID;
	_loggers[LOG4Z_MAIN_LOGGER_ID]._key = LOG4Z_MAIN_LOGGER_KEY;
	_loggers[LOG4Z_MAIN_LOGGER_ID]._name = _proName;
}
LogerManager::~LogerManager()
{
	stop();
}

void LogerManager::showColorText(const char *text, int level)
{
	if (level <= LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL)
	{
		printf("%s", text);
		return;
	}

#if !(defined(_WIN32) || defined(_WIN64))
	printf("%s%s\e[0m", LOG_COLOR[level], text);
#else

	Log4zAutoLocker l(_scLock);
	HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStd == INVALID_HANDLE_VALUE) {
		return;
	}
	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	if (!::GetConsoleScreenBufferInfo(hStd, &oldInfo)) {
		return;
	}
	else {
		::SetConsoleTextAttribute(hStd, LOG_COLOR[level]);
		printf("%s", text);
		::SetConsoleTextAttribute(hStd, oldInfo.wAttributes);
	}
#endif
	return;
}

bool LogerManager::configFromStringImpl(std::string content, bool isUpdate)
{
	std::map<std::string, LoggerInfo> loggerMap;

	if (!parseConfigFromString(content, loggerMap))
	{
		std::cout << " !!! !!! !!! !!!" << std::endl;
		std::cout << " !!! !!! log4z load config file error" << std::endl;
		std::cout << " !!! !!! !!! !!!" << std::endl;
		return false;
	}

	for (std::map<std::string, LoggerInfo>::iterator iter = loggerMap.begin(); iter != loggerMap.end(); ++iter)
	{
		LoggerId id = LOG4Z_INVALID_LOGGER_ID;
		id = findLogger(iter->second._key.c_str());

		if (id == LOG4Z_INVALID_LOGGER_ID) {
			if (isUpdate) {
				continue;
			}
			else {
				id = createLogger(iter->second._key.c_str());
				if (id == LOG4Z_INVALID_LOGGER_ID) {
					continue;
				}
			}
		}
		enableLogger(id, iter->second._enable);
		setLoggerName(id, iter->second._name.c_str());
		setLoggerPath(id, iter->second._path.c_str());
		setLoggerLevel(id, iter->second._level);
		setLoggerFileLine(id, iter->second._fileLine);
		setLoggerDisplay(id, iter->second._display);
		setLoggerOutFile(id, iter->second._outfile);
		setLoggerLimitsize(id, iter->second._limitsize);
		setLoggerMonthdir(id, iter->second._monthdir);
		setLoggerRebootDelete(id, iter->second._rebootDelete);
		setLoggerMaxLogFileCnt(id, iter->second._maxLogFileCnt);
		setLoggerFileClearCycleHour(id, iter->second._fileClearCycleHour);
	}
	return true;
}

//! read configure and create with overwriting
bool LogerManager::config(const char* configPath)
{
	if (!_configFile.empty()) {
		std::cout << " !!! !!! !!! !!!" << std::endl;
		std::cout << " !!! !!! log4z configure error: too many calls to Config. the old config file=" << _configFile << ", the new config file=" << configPath << " !!! !!! " << std::endl;
		std::cout << " !!! !!! !!! !!!" << std::endl;
		return false;
	}

	_configFile = configPath;

	Log4zFileHandler f;
	f.open(_configFile.c_str(), "rb");
	if (!f.isOpen()) {
		std::cout << " !!! !!! !!! !!!" << std::endl;
		std::cout << " !!! !!! log4z load config file error. filename=" << configPath << " !!! !!! " << std::endl;
		std::cout << " !!! !!! !!! !!!" << std::endl;
		return false;
	}

	return configFromStringImpl(f.readContent().c_str(), false);
}

//! read configure and create with overwriting
bool LogerManager::configFromString(const char* configContent)
{
	return configFromStringImpl(configContent, false);
}

//! create with overwriting
LoggerId LogerManager::createLogger(const char* key)
{
	if (key == NULL) {
		return LOG4Z_INVALID_LOGGER_ID;
	}

	std::string copyKey = key;
	trimLogConfig(copyKey);

	LoggerId newID = LOG4Z_INVALID_LOGGER_ID;
	{
		std::map<std::string, LoggerId>::iterator iter = _ids.find(copyKey);

		if (iter != _ids.end())
		{
			newID = iter->second;
		}
	}

	if (newID == LOG4Z_INVALID_LOGGER_ID)
	{
		if (_lastId + 1 >= LOG4Z_LOGGER_MAX)
		{
			showColorText("log4z: CreateLogger can not create|writeover, because loggerid need < LOGGER_MAX! \r\n", LOG_LEVEL_FATAL);
			return LOG4Z_INVALID_LOGGER_ID;
		}

		newID = ++_lastId;
		_ids[copyKey] = newID;
		_loggers[newID]._enable = true;
		_loggers[newID]._key = copyKey;
		_loggers[newID]._name = copyKey;
	}
	return newID;
}


bool LogerManager::start()
{
	if (_runing) {
		return false;
	}
	_semaphore.create(0);
	bool ret = ThreadHelper::start();
	return ret && _semaphore.wait(3000);
}
bool LogerManager::stop()
{
	if (_runing == true) {
		_runing = false;
		wait();
		return true;
	}

	return false;
}
bool LogerManager::prePushLog(LoggerId id, int level)
{
	if (id < 0 || id > _lastId || !_runing || !_loggers[id]._enable) {
		return false;
	}
	if (level < _loggers[id]._level) {
		return false;
	}
	return true;
}
bool LogerManager::pushLog(LoggerId id, int level, const char * log, const char * file, int line)
{
	// discard log
	if (id < 0 || id > _lastId || !_runing || !_loggers[id]._enable) {
		return false;
	}

	//filter log
	if (level < _loggers[id]._level) {
		return false;
	}

	//create log data
	LogData * pLog = new LogData;
	pLog->_id = id;
	pLog->_level = level;

	//append precise time to log
	{
#if defined(_WIN32) || defined(_WIN64)
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		unsigned long long now = ft.dwHighDateTime;
		now <<= 32;
		now |= ft.dwLowDateTime;
		now /= 10;
		now -= 11644473600000000ULL;
		now /= 1000;
		pLog->_time = now / 1000;
		pLog->_precise = (unsigned int)(now % 1000);
#else
		struct timeval tm;
		gettimeofday(&tm, NULL);
		pLog->_time = tm.tv_sec;
		pLog->_precise = tm.tv_usec / 1000;
#endif
	}

	//format log
	{
		tm tt = timeToTm(pLog->_time);

		if (file == NULL || !_loggers[pLog->_id]._fileLine)
		{
#if defined(_WIN32) || defined(_WIN64)
			int ret = _snprintf_s(pLog->_content, LOG4Z_LOG_BUF_SIZE, _TRUNCATE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s \r\n",
				tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
				LOG_STRING[pLog->_level], log);

			if (ret == -1)
			{
				ret = LOG4Z_LOG_BUF_SIZE - 1;
			}

			pLog->_contentLen = ret;
#else
			int ret = snprintf(pLog->_content, LOG4Z_LOG_BUF_SIZE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s \r\n",
				tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
				LOG_STRING[pLog->_level], log);

			if (ret == -1)
			{
				ret = 0;
			}

			if (ret >= LOG4Z_LOG_BUF_SIZE)
			{
				ret = LOG4Z_LOG_BUF_SIZE - 1;
			}

			pLog->_contentLen = ret;
#endif
		}
		else
		{
			const char * pNameBegin = file + strlen(file);
			do
			{
				if (*pNameBegin == '\\' || *pNameBegin == '/') {
					pNameBegin++;
					break;
				}

				if (pNameBegin == file) {
					break;
				}
				pNameBegin--;
			} while (true);

#if defined(_WIN32) || defined(_WIN64)
			int ret = _snprintf_s(pLog->_content, LOG4Z_LOG_BUF_SIZE, _TRUNCATE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s (%s):%d \r\n",
				tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
				LOG_STRING[pLog->_level], log, pNameBegin, line);

			if (ret == -1) {
				ret = LOG4Z_LOG_BUF_SIZE - 1;
			}
			pLog->_contentLen = ret;
#else
			int ret = snprintf(pLog->_content, LOG4Z_LOG_BUF_SIZE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s (%s):%d \r\n",
				tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
				LOG_STRING[pLog->_level], log, pNameBegin, line);

			if (ret == -1)
			{
				ret = 0;
			}

			if (ret >= LOG4Z_LOG_BUF_SIZE)
			{
				ret = LOG4Z_LOG_BUF_SIZE - 1;
			}

			pLog->_contentLen = ret;
#endif
		}

		if (pLog->_contentLen >= 2)
		{
			pLog->_content[pLog->_contentLen - 2] = '\r';
			pLog->_content[pLog->_contentLen - 1] = '\n';
		}
	}
	// 同步输出所有日志
	if (LOG4Z_SYNCOUTPUT_ALL_LOG)
	{
		if (_loggers[pLog->_id]._display)
		{
			showColorText(pLog->_content, pLog->_level);
#if defined(_WIN32) || defined(_WIN64)
			if (LOG4Z_ENABLE_DEBUG_OUTPUT) {
				OutputDebugStringA(pLog->_content);
			}
#endif
		}
		if (_loggers[pLog->_id]._outfile)
		{
			Log4zAutoLocker l(_logLock);
			if (openLogger(pLog))
			{
				_loggers[pLog->_id]._handle.write(pLog->_content, pLog->_contentLen);
				closeLogger(pLog->_id);
				_ullStatusTotalWriteFileCount++;
				_ullStatusTotalWriteFileBytes += pLog->_contentLen;
			}
		}
		delete pLog;
		pLog = nullptr;
	}
	else {
		Log4zAutoLocker l(_logLock);
		_logs.push_back(pLog);
		_ullStatusTotalPushLog++;
	}
	return true;
}

//! 查找ID
LoggerId LogerManager::findLogger(const char * key)
{
	std::map<std::string, LoggerId>::iterator iter;
	iter = _ids.find(key);
	if (iter != _ids.end()) {
		return iter->second;
	}
	return LOG4Z_INVALID_LOGGER_ID;
}
bool LogerManager::enableLogger(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	_loggers[id]._enable = enable;
	return true;
}
bool LogerManager::setLoggerLevel(LoggerId id, int level)
{
	if (id < 0 || id > _lastId || level < LOG_LEVEL_TRACE || level > LOG_LEVEL_FATAL)
	{
		return false;
	}

	_loggers[id]._level = level;
	return true;
}
bool LogerManager::setLoggerDisplay(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}
	_loggers[id]._display = enable;
	return true;
}
bool LogerManager::setLoggerOutFile(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	_loggers[id]._outfile = enable;
	return true;
}
bool LogerManager::setLoggerMonthdir(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	_loggers[id]._monthdir = enable;
	return true;
}
bool LogerManager::setLoggerLimitsize(LoggerId id, unsigned int limitsize)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	if (limitsize == 0)
	{
		limitsize = (unsigned int)-1;
	}

	_loggers[id]._limitsize = limitsize;
	return true;
}
bool LogerManager::setLoggerFileLine(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	_loggers[id]._fileLine = enable;
	return true;
}
bool LogerManager::setLoggerRebootDelete(LoggerId id, bool enable)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	_loggers[id]._rebootDelete = enable;
	return true;
}

bool LogerManager::setLoggerFileClearCycleHour(LoggerId id, int clearCycleHour)
{
	if (id < 0 || id > _lastId) {
		return false;
	}
	_loggers[id]._fileClearCycleHour = clearCycleHour;
	return true;
}

bool LogerManager::setLoggerMaxLogFileCnt(LoggerId id, int maxLogFileCnt)
{
	if (id < 0 || id > _lastId) {
		return false;
	}
	_loggers[id]._maxLogFileCnt = maxLogFileCnt;
	return true;
}

// 设置上一次清理文件的时间
bool LogerManager::setLoggerLastFileClearTime(LoggerId id, time_t time)
{
	if (id < 0 || id > _lastId) {
		return false;
	}
	_loggers[id]._lastFileClearTime = time;
	return true;
}

bool LogerManager::setLoggerStartTime(LoggerId id, time_t time)
{
	if (id < 0 || id > _lastId) {
		return false;
	}
	_loggers[id]._loggerStartTime = time;
	return true;
}

bool LogerManager::setLoggerName(LoggerId id, const char * name)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	// the name by main logger is the process name and it's can't change.
	if (id == LOG4Z_MAIN_LOGGER_ID)
	{
		return false;
	}

	if (name == NULL || strlen(name) == 0)
	{
		return false;
	}

	Log4zAutoLocker l(_hotLock);

	if (_loggers[id]._name != name)
	{
		_loggers[id]._name = name;
		_loggers[id]._hotChange = true;
	}
	return true;
}

bool LogerManager::setLoggerPath(LoggerId id, const char * path)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	std::string copyPath;

	if (path == NULL || strlen(path) == 0)
	{
		copyPath = LOG4Z_DEFAULT_PATH;
	}
	else
	{
		copyPath = path;
	}

	{
		char ch = copyPath.at(copyPath.length() - 1);

		if (ch != '\\' && ch != '/')
		{
			copyPath.append("/");
		}
	}

	Log4zAutoLocker l(_hotLock);

	if (copyPath != _loggers[id]._path)
	{
		_loggers[id]._path = copyPath;
		_loggers[id]._hotChange = true;
	}

	return true;
}
bool LogerManager::setAutoUpdate(int interval)
{
	_hotUpdateCfgInterval = interval;
	return true;
}
bool LogerManager::updateConfig()
{
	if (_configFile.empty())
	{
		//LOGW("log4z update config file error. filename is empty.");
		return false;
	}

	Log4zFileHandler f;
	f.open(_configFile.c_str(), "r");

	if (!f.isOpen())
	{
		std::cout << " !!! !!! !!! !!!" << std::endl;
		std::cout << " !!! !!! log4z load config file error. filename=" << _configFile << " !!! !!! " << std::endl;
		std::cout << " !!! !!! !!! !!!" << std::endl;
		return false;
	}

	return configFromStringImpl(f.readContent().c_str(), true);
}

bool LogerManager::isLoggerEnable(LoggerId id)
{
	if (id < 0 || id > _lastId)
	{
		return false;
	}

	return _loggers[id]._enable;
}

unsigned int LogerManager::getStatusActiveLoggers()
{
	unsigned int actives = 0;

	for (int i = 0; i <= _lastId; i++)
	{
		if (_loggers[i]._enable)
		{
			actives++;
		}
	}

	return actives;
}


bool LogerManager::openLogger(LogData * pLog)
{
	int id = pLog->_id;
	if (id < 0 || id > _lastId)
	{
		showColorText("log4z: openLogger can not open, invalide logger id! \r\n", LOG_LEVEL_FATAL);
		return false;
	}

	LoggerInfo * pLogger = &_loggers[id];
	if (!pLogger->_enable || !pLogger->_outfile || pLog->_level < pLogger->_level)
	{
		return false;
	}

	bool sameday = isSameDay(pLog->_time, pLogger->_curFileCreateTime);
	bool fileSizeOutLimit = pLogger->_curWriteLen > pLogger->_limitsize * 1024 * 1024;
	if (!sameday || fileSizeOutLimit || pLogger->_hotChange)
	{
		if (pLogger->_handle.isOpen())
		{
			pLogger->_handle.close();
		}
		if (!sameday)
		{
			pLogger->_curWriteLen = 0;
			pLogger->_curFileCreateTime = pLog->_time;
		}
		else if (fileSizeOutLimit)
		{
			std::string filePath = pLogger->_handle.getFilePath();
			std::string bakFilePath = pLogger->_handle.getBakPathPrefix();
			tm t = timeToTm(time(nullptr));
			char buf[256] = { 0 };
#if defined(WIN32) || defined(WIN64)
			sprintf_s(buf, sizeof(buf), "_Bak_%04d%02d%02d-%02d%02d%02d.log", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
#else
			sprintf(buf, "_Bak_%04d%02d%02d-%02d%02d%02d.log", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
#endif
			bakFilePath += buf;
			rename(filePath.c_str(), bakFilePath.c_str());
			pLogger->_curWriteLen = 0;
		}
	}

	if (!pLogger->_handle.isOpen())
	{
		tm t = timeToTm(pLogger->_curFileCreateTime);
		std::string name("");
		std::string path("");
		_hotLock.lock();
		name = pLogger->_name;
		path = pLogger->_path;
		pLogger->_hotChange = false;
		_hotLock.unLock();

		char buf[256] = { 0 };
		if (pLogger->_monthdir)
		{
			sprintf(buf, "%04d_%02d/", t.tm_year + 1900, t.tm_mon + 1);
			path += buf;
		}

		if (!isDirectory(path))
		{
			createRecursionDir(path);
		}
		// 设置日志文件的保存路径
		path += name;
		pLogger->_handle.setBakPathPrefix(path);
		// 使用按天 + 文件编号的文件名
		sprintf(buf, "_Log_%04d%02d%02d.log", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
		path += buf;
		pLogger->_handle.open(path.c_str(), "ab");
		if (!pLogger->_handle.isOpen())
		{
			pLogger->_outfile = false;
			return false;
		}

		//  记录文件头
		tm startDateTime = timeToTm(pLogger->_loggerStartTime);
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "---------------create new log file, logger name: %s, starttime: %04d-%02d-%02d %02d:%02d:%02d-------------\r\n",
			pLogger->_key.c_str(), startDateTime.tm_year + 1900, startDateTime.tm_mon + 1, startDateTime.tm_mday,
			startDateTime.tm_hour, startDateTime.tm_min, startDateTime.tm_sec);
		pLogger->_handle.write(buf, strlen(buf));
		pLogger->_curWriteLen = pLogger->_handle.getSize();

		// 系统下次启动时,删除该日志文件
		if (pLogger->_rebootDelete) {
#if defined(_WIN32) || defined(_WIN64)
			// Remark:MOVEFILE_DELAY_UNTIL_REBOOT 执行需要管理员权限
			BOOL bRet = ::MoveFileExA(path.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			if (bRet == FALSE) {
				showColorText("log4z:mark the logfile reboot delete faild! \r\n", LOG_LEVEL_FATAL);
				if (5 == ::GetLastError()) {
					showColorText("log4z:need Administrator Auth.\r\n", LOG_LEVEL_FATAL);
				}
			}
#endif
		}
		return true;
	}
	return true;
}
bool LogerManager::closeLogger(LoggerId id)
{
	if (id < 0 || id > _lastId) {
		showColorText("log4z: closeLogger can not close, invalide logger id! \r\n", LOG_LEVEL_FATAL);
		return false;
	}

	LoggerInfo * pLogger = &_loggers[id];
	if (pLogger->_handle.isOpen())
	{
		pLogger->_handle.close();
		return true;
	}
	return false;
}
bool LogerManager::popLog(LogData *& log)
{
	Log4zAutoLocker l(_logLock);

	if (_logs.empty())
	{
		return false;
	}

	log = _logs.front();
	_logs.pop_front();
	return true;
}

// 日志文件的删除策略如下：
// 1、若日志文件已过期，则必须删除
// 2、若日志文件未过期，则删除多余的日志文件
// 3、默认每24小时检测1次，若待删除的日志文件增多，则动态调整文件的删除周期
void LogerManager::clearExpireLogFiles(const int loggerId)
{
	// 日志ID无效,或者日志未启用
	if (loggerId < 0 || loggerId > _lastId || !_loggers[loggerId]._enable) {
		return;
	}

	// 搜索日志路径下的.log文件
	std::string logDirPattern = _loggers[loggerId]._path;
	int len = logDirPattern.length();
	if (len <= 0) {
		return;
	}
	logDirPattern.append("*.log");
	std::vector<std::pair<int64_t, std::string>> logFiePaths(0);
#if defined(WIN32) || defined(WIN64)
	_finddata_t fd = { 0 };
	intptr_t pf = _findfirst(logDirPattern.c_str(), &fd);
	if (pf < 0) {
		return;
	}
	// 枚举日志目录下的所有日志文件
	int ret = 0;
	while (ret >= 0) {
		// 不删除特殊文件或文件夹
		if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0 || fd.attrib == _A_SUBDIR) {
			ret = _findnext(pf, &fd);
			continue;
		}

		// 带路径的文件名称
		std::string filePath = _loggers[loggerId]._path;
		filePath.append(fd.name);
		logFiePaths.push_back(std::make_pair((int64_t)(fd.time_write), filePath));
		ret = _findnext(pf, &fd);
	}
	_findclose(pf);
#else
	DIR* d = opendir(logDirPattern.c_str());
	if (d == nullptr) {
		return;
	}
	struct dirent* dp = readdir(d);
	while (dp != nullptr)
	{
		if ((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2))) {
			dp = readdir(d);
			continue;
		}
		// 带路径的文件名称
		std::string filePath = _loggers[loggerId]._path;
		filePath.append(dp->d_name);
		struct stat st = { 0 };
		if (stat(filePath.c_str(), &st) == 0) {
			logFiePaths.push_back(std::make_pair((int64_t)(st.st_mtim.tv_sec), filePath));
		}
		dp = readdir(d);
	}
#endif

	// 统计需要删除的日志文件
	int deleteCnt = (int)logFiePaths.size() - _loggers[loggerId]._maxLogFileCnt;
	if (deleteCnt <= 0) {
		return;
	}
	std::sort(logFiePaths.begin(), logFiePaths.end(),
		[](const std::pair<int64_t, std::string> &x, const std::pair<int64_t, std::string> &y) {
		return x.first < y.first;
	});

	for (int i = 0; i < deleteCnt; i++) {
		std::string &delFilePath = logFiePaths[i].second;
		remove(delFilePath.c_str());
	}
}

// 日志处理主线程
void LogerManager::run()
{
	// 日志线程启动，初始化
	_runing = true;
	std::stringstream ssTips;
#if defined(WIN32) || defined(WIN64)
	uint64_t curThreadId = GetCurrentThreadId();
#else
	uint64_t curThreadId = (uint64_t)pthread_self();
#endif
	ssTips << "----------------- log4z thread started! -----------" << "log write thread id=" << curThreadId << "-----------------";
	pushLog(0, LOG_LEVEL_ALARM, ssTips.str().c_str(), NULL, 0);
	time_t currentTime = time(nullptr);
	for (int i = 0; i <= _lastId; i++)
	{
		if (!_loggers[i]._enable) {
			continue;
		}
		std::stringstream ss;
		ss << "logger id=" << i
			<< " key=" << _loggers[i]._key
			<< " name=" << _loggers[i]._name
			<< " path=" << _loggers[i]._path
			<< " level=" << _loggers[i]._level
			<< " display=" << _loggers[i]._display
			<< " rebootdelete=" << _loggers[i]._rebootDelete
			<< " limitsize=" << _loggers[i]._limitsize << "M"
			<< " maxlogfilecnt=" << _loggers[i]._maxLogFileCnt
			<< " fileclearcycle=" << _loggers[i]._fileClearCycleHour << "H";
		pushLog(0, LOG_LEVEL_ALARM, ss.str().c_str(), NULL, 0);
		// logger初始化
		setLoggerStartTime(i, currentTime);
		clearExpireLogFiles(i);
		setLoggerLastFileClearTime(i, currentTime);
	}
	_semaphore.post();// 日志线程启动完成

	time_t lastCheckCfgUpdate = time(NULL);
	// 初始化完成，进入循环
	while (true)
	{
		LogData * pLog = NULL;
		while (popLog(pLog))
		{
			_ullStatusTotalPopLog++;
			LoggerInfo & curLogger = _loggers[pLog->_id];
			// 该日志文件不需要输出
			// 所有日志文件已经同步输出
			if (!curLogger._enable || pLog->_level < curLogger._level || LOG4Z_SYNCOUTPUT_ALL_LOG)
			{
				delete pLog;
				pLog = NULL;
				continue;
			}
			// 将日志文件输出到console
			if (curLogger._display) {
				showColorText(pLog->_content, pLog->_level);
#if defined(_WIN32) || defined(_WIN64)
				if (LOG4Z_ENABLE_DEBUG_OUTPUT) {
					::OutputDebugStringA(pLog->_content); // 将日志输出到调试终端
				}
#endif
			}

			// 输出日志到文件
			if (curLogger._outfile)
			{
				if (!openLogger(pLog)) // 创建日志文件失败
				{
					delete pLog;
					pLog = NULL;
					continue;
				}

				curLogger._handle.write(pLog->_content, pLog->_contentLen);
				curLogger._curWriteLen += (unsigned int)pLog->_contentLen;
				curLogger._needFlushLogCnt = 0;

				// 若日志过多,则强制刷新一次
				if (curLogger._needFlushLogCnt > 1024) {
					curLogger._handle.flush();
					curLogger._needFlushLogCnt = 0;
				}
				_ullStatusTotalWriteFileCount++;
				_ullStatusTotalWriteFileBytes += pLog->_contentLen;
			}
			delete pLog;
			pLog = NULL;
		}
		for (int i = 0; i <= _lastId; i++)
		{
			if (_loggers[i]._enable) {
				// 刷新日志文件
				if (_loggers[i]._needFlushLogCnt > 0 && _loggers[i]._handle.isOpen()) {
					_loggers[i]._handle.flush();
					_loggers[i]._needFlushLogCnt = 0;
				}
			}
			else {
				// 关闭日志文件
				if (_loggers[i]._handle.isOpen()) {
					_loggers[i]._handle.close();
				}
			}
		}
		// 删除过期的日期文件
		time_t nowTime = time(nullptr);
		for (int i = 0; i <= _lastId; i++) {
			if (!_loggers[i]._enable) {
				continue;
			}
			time_t diffTime = nowTime - _loggers[i]._lastFileClearTime;
			if (diffTime > (_loggers[i]._fileClearCycleHour * 3600)) {
				clearExpireLogFiles(i);
				_loggers[i]._lastFileClearTime = nowTime;
			}
		}

		//! delay.
		sleepMillisecond(50);

		//! 退出日志线程,必须将所有日志都写入文件
		if (!_runing && _logs.empty())
		{
			break;
		}
		// 更新日志配置
		time_t currentTime = time(NULL);
		if (_hotUpdateCfgInterval > 0 && currentTime - lastCheckCfgUpdate > _hotUpdateCfgInterval)
		{
			updateConfig();
			lastCheckCfgUpdate = currentTime;
		}
	}

	// 退出阶段，关闭已打开的日志
	for (int i = 0; i <= _lastId; i++)
	{
		if (_loggers[i]._enable)
		{
			_loggers[i]._enable = false;
			closeLogger(i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//ILog4zManager::getInstance
//////////////////////////////////////////////////////////////////////////
LogerManager * LogerManager::instance = NULL;

// 创建日志管理模块单例
ILog4zManager * ILog4zManager::getInstance()
{
	if (LogerManager::instance == NULL) {
		LogerManager::instance = new LogerManager;
	}
	return LogerManager::instance;
}

void ILog4zManager::freeInstance()
{
	if (LogerManager::instance != NULL) {
		LogerManager::instance->stop();
		delete LogerManager::instance;
		LogerManager::instance = NULL;
	}
}
_ZSUMMER_LOG4Z_END
_ZSUMMER_END
