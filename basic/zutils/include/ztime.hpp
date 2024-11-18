#pragma once
#include "zutils_sdk.h"
#if defined(WIN32) || defined(WIN64)
#include <stdint.h>
#else
#endif
#include <ctime>
#include "zstring.h"
#include<chrono>

//经测试，MSVC不支持宏函数返回值的写法，考虑兼容性，先放弃,修改为强制内联函数
//#define GET_CURRENT_TIME() \
//({							\
//uint64_t currentTime = 0;		\
//auto curTime = std::chrono::steady_clock::now();		\
//auto curMillSec = std::chrono::time_point_cast<std::chrono::milliseconds>(curTime);	\
//currentTime = curMillSec.time_since_epoch().count();	\
//currentTime;					\
//})								

// 定义一个宏，用于跨编译器的内联函数
#if defined(_MSC_VER) // 如果是 Microsoft Visual C++ 编译器
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__) // 如果是 GCC 或 Clang 编译器
#define FORCE_INLINE __attribute__((always_inline)) inline
#else // 其他编译器
#define FORCE_INLINE inline
#endif

//考虑到MSVC的兼容性，先写成这样(这个函数会用来计算延迟任务，所以对其实时性要求比较高，强制内联)
FORCE_INLINE uint64_t _getCurrentTime_() {
	uint64_t currentTime = 0;		
	auto curTime = std::chrono::steady_clock::now();		
	auto curMillSec = std::chrono::time_point_cast<std::chrono::milliseconds>(curTime);	
	currentTime = curMillSec.time_since_epoch().count();	
	return currentTime;					
}


ZUTILS_NS_BEGIN

class ZUTILS_API ZTime {
public:
	ZTime();
	~ZTime();


	static void getNowTimeMilliSecStr(ZString& nowTime);
	static time_t getNowTimeMilliSec();
	static void timeMilliSecToStr(const time_t timeMSec, ZString& strTime);

};
ZUTILS_NS_END