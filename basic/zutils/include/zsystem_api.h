#pragma once
#include "zutils_sdk.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
//#include "aastring.h"
/**********************************************************************
 *@file   aasystem_api.h
 *@date   2022/10/26 20 : 53
 *@author shanwenbin
 *@brief  跨平台系统api
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZSystemApi
{
public:
	/**
	  @brief 获取当前线程的ID
	  @return 当前线程的ID
	 */
	static unsigned long getCurrentThreadId();

	/**
	  @brief 获取当前进程的ID
	  @return　当前进程的ID
	 */
	static int getCurrentProcId();

	/**
	 @brief 获取当前进程的名称
	 @param[out] procName 当前进程的名称
	 @return 无
	 */
//	static void getCurrentProcName(AAString& procName);

	/**
	 @brief 获取当前进行所在的文件夹
	 @param[in] procDir 当前进程所在的文件夹
	 @return 无
	 */
//	static void getCurrentProcDir(AAString& procDir);

	/**
	  @brief 设置程序的工作目录
	  @param[in] workingDir 程序的工作目录
	  @return 无
	 */
	static void setWorkingDir(const char* workingDir);

	/**
	 @brief 获取本机文档目录
	 @param[out] docDir 本机的document目录
	 @return 无
	 */
//	static void getDocumentDir(AAString& docDir);

	/**
	 @brief 获取当前用户主目录, linux为home目录, windows为AppData目录
	 @param[out] appDataDir 本机的AppData目录
	 @return 无
	 */
//	static void getUserHomeDir(AAString& appDataDir);

	/**
	 @brief 生成一个位于 [from, to]的整数，包括from和to
	 @param[in] from 随机数的下限
	 @param[in] from 随机数的上限
	 @return 满足要求的随机数
	 */
	static int random(const int from, const int to);

	/**
	 @brief 获取系统中CPU的数量
	 @return PC唯一标识
	 */
	static unsigned int getCpuCnt();

protected:
	ZSystemApi() = default;
	virtual ~ZSystemApi() = default;
};
ZUTILS_NS_END