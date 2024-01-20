#pragma once
#include "zutils_sdk.h"
#include <atomic>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <semaphore.h>
#include <unistd.h>
#endif
/**********************************************************************
 *@brief  事件锁(信号锁),  windows下基于event实现, linux下基于sem实现
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZEventLock
{
public:
	ZEventLock();
	virtual ~ZEventLock();
public:
	/**
	 @brief 创建信号锁
	 @param[in] isSetted 初始状态下是否处于触发状态
	 @param[in] isManualReset 是否需要手动重置
	 @return true:创建成功, flase 创建失败
	 */
	bool create(const bool isSetted = false, const bool isManualReset = false);

	/**
	 @brief 销毁信号锁
	 @return 无
	 */
	void destroy();

	/**
	 @brief 将信号锁设置为触发状态(多次设置,次数不可累计)
	 @return 无
	 */
	void setEvent();

	/**
	 @brief 将信号锁设置为未触发状态
	 @return 无
	 */
	void resetEvent();

	/**
	 @brief 等待事件直到被触发为止(会阻塞当前线程)
	 @return 无
	 */
	void waitEvent();

	/**
	 @brief 等待事件直到超时或事件被触发为止
	 @param[in] timeoutMs 超时时间(毫秒)
	 @return true:事件被触发, false:等待超时
	 */
	bool tryEvent(const int timeoutMs = 0);
private:
	bool isManualReset_{ false }; /**< 是否需要手动重置 */
	bool isCreated_{ false }; /**< 信号锁是否已成功创建 */
#if defined(WIN32) || defined(WIN64)
	HANDLE evtHandler_{ nullptr }; /**< 信号锁句柄(windows) */
#else
	sem_t evtHandler_;  /**< 信号锁句柄(linux) */
	std::atomic<int> evtWakedCnt_{ 0 }; /**< 信号锁被唤醒的次数 */
#endif
};
ZUTILS_NS_END
