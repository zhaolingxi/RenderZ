//#include "logic/logic_module_evtdispatcher.h"
//ZCORE_NS_BEGIN
//bool LogicModuleEvtDispatcher::registerEvtNotifyCB(const int64_t evtId, const int64_t routerId, LogicModuleEvtNotifyCB notifyCB)
//{
//	EvtNotifyCBListPtr procList = nullptr;
//	auto it = evtIdToEvtCBListMap_.find(evtId);
//	if (it != evtIdToEvtCBListMap_.end()) {
//		procList = it->second;
//	}
//	else {
//		procList = std::make_shared<EvtNotifyCBList>();
//		evtIdToEvtCBListMap_.insert(std::make_pair(evtId, procList));
//	}
//	if (procList == nullptr) {
//		return false;
//	}
//	auto evtCBPair = std::make_shared<RouterNotifyCBPair>();
//	evtCBPair->routerId = routerId;
//	evtCBPair->notifyCB = notifyCB;
//	procList->push_back(evtCBPair);
//	return true;
//}
//
//void LogicModuleEvtDispatcher::unregisterEvtNotifyCB(const int64_t evtId, const int64_t routerId)
//{
//	auto itList = evtIdToEvtCBListMap_.find(evtId);
//	if (itList == evtIdToEvtCBListMap_.end()) {
//		return;
//	}
//	auto procList = itList->second;
//	if (procList == nullptr) {
//		return;
//	}
//	for (auto it = procList->begin(); it != procList->end(); ) {
//		if ((*it)->routerId == routerId) {
//			it = procList->erase(it);
//			continue;
//		}
//		++it;
//	}
//}
//
//void LogicModuleEvtDispatcher::unregisterEvtNotifyCB(const int64_t routerId)
//{
//	for (auto& item : evtIdToEvtCBListMap_) {
//		auto procList = item.second;
//		if (procList == nullptr) {
//			continue;
//		}
//		for (auto it = procList->begin(); it != procList->end(); ) {
//			if ((*it)->routerId == routerId) {
//				it = procList->erase(it);
//				continue;
//			}
//			++it;
//		}
//	}
//}
//
//bool LogicModuleEvtDispatcher::dispatchEvtNotify(const int64_t evtId, const std::any& data)
//{
//	auto itList = evtIdToEvtCBListMap_.find(evtId);
//	if (itList == evtIdToEvtCBListMap_.end()) {
//		return false;
//	}
//	auto procList = itList->second;
//	if (procList == nullptr) {
//		return false;
//	}
//
//	for (auto it = procList->begin(); it != procList->end(); ++it) {
//		auto evtCB = (*it)->notifyCB;
//		if (evtCB == nullptr) {
//			continue;
//		}
//		evtCB(evtId, data);
//	}
//	return true;
//}
//ZCORE_NS_END
