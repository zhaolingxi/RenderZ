#pragma once
#include "zutils_sdk.h"
#include <cstdio>
#include <string>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif
/**********************************************************************
 *@file   aalib_loader.h
 *@date   2022/11/23 12 : 29
 *@brief  动态库加载器
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZLibLoader
{
public:
#if defined(WIN32) || defined(WIN64) 
	typedef HMODULE ModHandler;  // 模块句柄
	typedef FARPROC ProcAddress; // 导出函数地址
#else
	typedef void* ModHandler;  // 模块句柄
	typedef void* ProcAddress; // 导出函数地址
#endif
public:
	ZLibLoader(const char* libFilePath);
	virtual ~ZLibLoader();

public:
	/**
	  @brief 加载动态库
	  @return true/false
	 */
	bool load();

	/**
	  @brief 动态库是否已加载
	  @return true/false
	 */
	bool isLoaded();

	/**
	  @brief 获取动态库的导出函数
	  @param[in] procName 导出函数的符好名称
	  @return 导出函数地址
	 */
	ProcAddress resolve(const char* procName);
	template<typename FnType>
	FnType resolveCast(const char* procName) {
		return reinterpret_cast<FnType>(resolve(procName));
	}

	/**
	  @brief 卸载动态库插件
	  @return 无
	 */
	void unload();

	/**
	  @brief 加载动态库
	  @param[in] libFilePath 动态库路径
	  @return 动态库句柄
	 */
	static ModHandler loadLib(const char* libFilePath);

	/**
	  @brief 获取导出函数地址
	  @param[in] modHandler 动态库句柄
	  @param[in] procName 导出函数名称
	  @return 导出函数地址
	 */
	static ProcAddress getProcAddress(ModHandler modHandler, const char* procName);
	template<typename FnType>
	static FnType getProcAddrCast(ModHandler modHandler, const char* procName) {
		return reinterpret_cast<FnType>(getProcAddress(modHandler, procName));
	}

	/**
	  @brief 释放动态库
	  @param[in] modHandler 动态库句柄
	  @return 无
	 */
	static void freeLib(ModHandler modHandler);

private:
	// 动态库文件路径
	std::string libFilePath_{ "" };
	// 模块动态库句柄
	ModHandler modHandler_{ nullptr };
};
ZUTILS_NS_END
