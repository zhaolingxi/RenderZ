#pragma once
#include "zcore_sdk.h"
#include <cstdint>
#include <cstdio>
#include "logic/logic_module_evtdispatcher.h"
ZCORE_NS_BEGIN
class ZCORE_API LogicModuleNotifyRouter
{
public:
	explicit LogicModuleNotifyRouter() = default;
	virtual ~LogicModuleNotifyRouter() = default;
public:
	void setRouterIdentity(const int64_t routerId);

	const int64_t getRouterIdentity() const;

	void setLogicEvtDispatcher(LogicModuleEvtDispatcher* evtDispatcher);

	/**
 	  @brief 绑定事件通知回调
 	  @param[in] evtId 事件ID
 	  @param[in] notifyCB 事件通知回调
 	  @return true/false
 	 */
	bool bindModuleEvtNotifyCB(const int64_t evtId, LogicModuleEvtNotifyCB notifyCB);
	// 适配enum类型
	template<typename Type>
	bool bindModuleEvtNotifyCB(const Type evtId, LogicModuleEvtNotifyCB notifyCB) {
		return bindModuleEvtNotifyCB(static_cast<int64_t>(evtId), notifyCB);
	}

	/**
 	  @brief 绑定事件通知回调(多个事件)
 	  @param[in] evtIds 事件ID列表
 	  @param[in] notifyCB 事件通知回调
 	  @return true/false
 	 */
	bool bindModuleEvtsNotifyCB(const std::vector<int64_t>& evtIds, LogicModuleEvtNotifyCB notifyCB);

	/**
 	  @brief 绑定事件通知回调
	  @note  业务模块的所有事件通知不区分局部和全局通知,全都是全局分发的
 	  @param T 回调参数类型
	  @param[in] evtId 事件ID
 	  @param[in] evtProc 事件通知回调
 	  @return true/false
 	 */
	template<class T>
	void bindModuleEvtNotifyCB(const int64_t evtId, std::function<void(const T& data)> notifyCB)
	{
		bindModuleEvtNotifyCB(evtId,
			[this, notifyCB](const int64_t evtId, const std::any& data)->void
			{
				// 不存在指导类型的数据
				if (!data.has_value() || data.type() != typeid(T)) {
					fprintf(stderr, "notifyCB no evtdata or type not same, classname:%s\n", aa_get_obj_tpname(this).c_str());
					throw std::runtime_error("notifyCB no evtdata or type not same");
				}
				const T& d = std::any_cast<const T&>(data);
				return notifyCB(d);
			});
	}

	bool sendModuleEvtNotify(const int64_t evtId, const std::any& data = std::any());
	// 适配enum类型
	template<typename Type>
	bool sendModuleEvtNotify(const Type evtId, const std::any& data = std::any()) {
		return sendModuleEvtNotify(static_cast<int64_t>(evtId), data);
	}

	bool postModuleEvtNotify(const int64_t evtId, const std::any& data = std::any());
	// 适配enum类型
	template<typename Type>
	bool postModuleEvtNotify(const Type evtId, const std::any& data = std::any()) {
		return postModuleEvtNotify(static_cast<int64_t>(evtId), data);
	}

	/**
 	 @brief 发送模块的事件通知到其他业务模块
 	 @param[in] evtId 事件ID
 	 @param[in] data 通知数据
 	 @return true/false
 	*/
	bool notifyModuleEvt(const int64_t evtId, const std::any& data = std::any());
	// 适配enum类型
	template<typename Type>
	bool notifyModuleEvt(const Type evtId, const std::any& data = std::any()) {
		return notifyModuleEvt(static_cast<int64_t>(evtId), data);
	}

private:
	int64_t routerIdentity_{ 0 }; /**< 通知路由的唯一标识 */
	LogicModuleEvtDispatcher* logicEvtDispatcher_{ nullptr }; /**< LogicModule事件分发器 */
};
ZCORE_NS_END
