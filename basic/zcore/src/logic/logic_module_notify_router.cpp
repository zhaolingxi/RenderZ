//#include "logic/logic_module_notify_router.h"
//#include "task_scheduler.h"
//#include "aacore_singleton.h"
//#include "log4z.h"
//#include "moduleid_tratis64.hpp"
//ZCORE_NS_BEGIN
//void LogicModuleNotifyRouter::setRouterIdentity(const int64_t routerId)
//{
//	routerIdentity_ = routerId;
//}
//
//const int64_t LogicModuleNotifyRouter::getRouterIdentity() const
//{
//	return routerIdentity_;
//}
//
//void LogicModuleNotifyRouter::setLogicEvtDispatcher(LogicModuleEvtDispatcher* evtDispatcher)
//{
//	logicEvtDispatcher_ = evtDispatcher;
//}
//
//bool LogicModuleNotifyRouter::bindModuleEvtNotifyCB(const int64_t evtId, LogicModuleEvtNotifyCB notifyCB)
//{
//	if (!AACoreSingleton::isRunningInMainThread()) {
//		LOGFMTE("bindModuleEvtNotifyCB() only allow call in main thread");
//		return false;
//	}
//	if (logicEvtDispatcher_ == nullptr) {
//		return false;
//	}
//	return logicEvtDispatcher_->registerEvtNotifyCB(evtId, getRouterIdentity(), notifyCB);
//}
//
//bool LogicModuleNotifyRouter::bindModuleEvtsNotifyCB(const std::vector<int64_t>& evtIds, LogicModuleEvtNotifyCB notifyCB)
//{
//	bool ret = true;
//	for (auto& evtId : evtIds) {
//		ret = bindModuleEvtNotifyCB(evtId, notifyCB);
//		if (!ret) {
//			LOGFMTE("bindModuleEvtsNotifyCB() bind evtId = %lld failed", evtId);
//			break;
//		}
//	}
//	return ret;
//}
//
//bool LogicModuleNotifyRouter::sendModuleEvtNotify(const int64_t evtId, const std::any& data /*= std::any()*/)
//{
//	// 主线程直接分发
//	if (logicEvtDispatcher_ == nullptr) {
//		return false;
//	}
//	//LOGFMTD("sendModuleEvtNotify() evtId = %lld [tag:%s, val:%d] thisPtr=%lld, classname: %s",
//	//	evtId, module_uppertag64(evtId).c_str(),
//	//	pure_enumval64(evtId), reinterpret_cast<int64_t>(this),
//	//	aa_get_obj_tpname(this).c_str());
//	return logicEvtDispatcher_->dispatchEvtNotify(evtId, data);
//}
//
//bool LogicModuleNotifyRouter::postModuleEvtNotify(const int64_t evtId, const std::any& data /*= std::any()*/)
//{
//	if (logicEvtDispatcher_ == nullptr) {
//		return false;
//	}
//	auto mainScheduler = TaskScheduler::getMainThreadScheduler();
//	if (mainScheduler == nullptr) {
//		return false;
//	}
//	mainScheduler->postTask([this, evtId, data]() {
//		sendModuleEvtNotify(evtId, data);
//	});
//	return true;
//}
//
//bool LogicModuleNotifyRouter::notifyModuleEvt(const int64_t evtId, const std::any& data /*= std::any()*/)
//{
//	if (logicEvtDispatcher_ == nullptr) {
//		return false;
//	}
//	bool ret = false;
//	if (AACoreSingleton::isRunningInMainThread()) {
//		ret = sendModuleEvtNotify(evtId, data);
//	}
//	else {
//		ret = postModuleEvtNotify(evtId, data);
//	}
//	return ret;
//}
//ZCORE_NS_END
