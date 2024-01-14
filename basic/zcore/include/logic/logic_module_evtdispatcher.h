#pragma once
#include "zcore_sdk.h"
#include <any>
#include <list>
#include <functional>
#include <memory>
/**********************************************************************
 *@file  logic_module_evtdispatcher.h
 *@date  2023/07/03 17 : 29
 *@author shanwenbin
 *@brief 业务模块的事件分发器
 ***********************************************************************/
ZCORE_NS_BEGIN
/**
  @brief 业务模块的事件通知回调
  @param[in] evtId 事件通知ID
  @param[in] data 事件关联的数据
  @return 无
*/
typedef std::function<void(const int64_t evtId, const std::any& data)> LogicModuleEvtNotifyCB;

// 事件回调绑定器
#define LOGICMOD_EVTNOTIFYCB_BINDER(func, thisPtr) \
	std::bind(func, thisPtr, std::placeholders::_1, std::placeholders::_2)

// 业务模块的事件分发器
class ZCORE_API LogicModuleEvtDispatcher
{
public:
	// 事件路由与事件回调的关联对
	struct RouterNotifyCBPair
	{
		int64_t routerId{ 0 };
		LogicModuleEvtNotifyCB notifyCB;
	};
	// 模块的事件通知回调列表
	using EvtNotifyCBList = std::list<std::shared_ptr<RouterNotifyCBPair>>;
	using EvtNotifyCBListPtr = std::shared_ptr<EvtNotifyCBList>;
public:
	explicit LogicModuleEvtDispatcher() = default;
	virtual ~LogicModuleEvtDispatcher() = default;
public:
	bool registerEvtNotifyCB(const int64_t evtId, const int64_t routerId, LogicModuleEvtNotifyCB notifyCB);
	void unregisterEvtNotifyCB(const int64_t evtId, const int64_t routerId);
	void unregisterEvtNotifyCB(const int64_t routerId);
	bool dispatchEvtNotify(const int64_t evtId, const std::any& data = std::any());
private:
	// evtId <---> EvtNotifyCBList(包含routerId和事件通知回调)
	std::unordered_map<int64_t, EvtNotifyCBListPtr> evtIdToEvtCBListMap_;
};
ZCORE_NS_END