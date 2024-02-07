#include "ztime.hpp"
ZUTILS_NS_BEGIN
ZTime::ZTime()
{
}

void ZTime::getNowTimeMilliSecStr(ZString& nowTime)
{
	time_t msec = getNowTimeMilliSec();
	timeMilliSecToStr(msec, nowTime);
}

time_t ZTime::getNowTimeMilliSec()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return millis;
}

void ZTime::timeMilliSecToStr(const time_t timeMSec, ZString& strTime)
{
	time_t sec = timeMSec / 1000;
	int msec = timeMSec % 1000;

	struct tm* tmTime = nullptr;
#ifdef _MSVC
	struct tm tmlocal;
	if (localtime_s(&tmlocal, &sec) == 0) {
		tmTime = &tmlocal;
	}
#else
	tmTime = localtime(&sec);
#endif
	if (tmTime == nullptr) {
		return;
	}
	strTime.ensureBuffer(128);
	size_t len = strftime((char*)strTime.getData(), strTime.getBufferSize(), "%Y-%m-%d %H:%M:%S", tmTime);
	strTime.setLength(len);
	// Ìî³äºÁÃë
	int pos = strTime.find("msc", 0);
	if (pos >= 0) {
		for (int i = 2; i >= 0; i--) {
			strTime.setAt(pos + i, (msec % 10) + '0');
			msec = msec / 10;
		}
	}
}
ZUTILS_NS_END