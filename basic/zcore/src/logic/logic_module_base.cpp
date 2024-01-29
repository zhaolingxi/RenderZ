//#pragma once
//#include <future>
//#include "log4z.h"
//#include "logic/logic_module_base.h"
//#include "logic/logic_module_manager.h"
//#include "config/config_manager.h"
//#include "aacore_singleton.h"
//
//ZCORE_NS_BEGIN
//LogicModuleBase::LogicModuleBase()
//{
//
//}
//
//LogicModuleBase::~LogicModuleBase()
//{
//
//}
//
//void LogicModuleBase::setIdentity(const int64_t modIdentity)
//{
//	moduleIdentity_ = modIdentity;
//}
//
//void LogicModuleBase::setInstIdx(const uint8_t instIdx)
//{
//	instIdx_ = instIdx;
//}
//
//void LogicModuleBase::setManager(LogicModuleManager* moduleManager, ConfigManager* configManager)
//{
//	moduleManager_ = moduleManager;
//	configManager_ = configManager;
//	if (moduleManager_ != nullptr) {
//		int64_t routerId = moduleManager_->getModuleInstUID(moduleIdentity_, instIdx_);
//		setRouterIdentity(routerId);
//		setLogicEvtDispatcher(moduleManager_);
//	}
//}
//
//std::shared_ptr<ConfigBase> LogicModuleBase::getConfigByName(const char* pluginName)
//{
//	if (configManager_ == nullptr || std::string::isEmptyStr(pluginName)) {
//		return nullptr;
//	}
//	return configManager_->getLogicConfigByName(pluginName);
//}
//
//std::shared_ptr<LogicModuleBase> LogicModuleBase::getModuleByIdentity(const int64_t moduleId, const uint8_t instIdx /*= 0*/)
//{
//	if (moduleManager_ == nullptr) {
//		return nullptr;
//	}
//	return moduleManager_->getModuleByIdentity(moduleId, instIdx);
//}
//
//int64_t LogicModuleBase::getIdentity() const
//{
//	return moduleIdentity_;
//}
//
//uint8_t LogicModuleBase::getInstIdx() const
//{
//	return instIdx_;
//}
//
//void LogicModuleBase::setConfig(std::shared_ptr<ConfigBase> config)
//{
//	config_ = config;
//}
//
//std::shared_ptr<ConfigBase> LogicModuleBase::getConfig()
//{
//	return config_;
//}
//
//bool LogicModuleBase::initModule()
//{
//	bool ret = true;
//	if (getBindTaskScheduler() == nullptr) {
//		ret = bindTaskScheduler(TaskScheduler::getCommLogicScheduler());
//	}
//	return ret;
//}
//
//void LogicModuleBase::stopModule()
//{
//	if (taskExecuter_ != nullptr) {
//		taskExecuter_ = nullptr;
//	}
//}
//
//void LogicModuleBase::exitModule()
//{
//	taskExecuter_ = nullptr;
//}
//
//bool LogicModuleBase::bindTaskScheduler(TaskScheduler* taskScheduler)
//{
//	if (taskExecuter_ == nullptr) {
//		taskExecuter_ = std::make_shared<TaskExecuter>();
//		taskExecuter_->setIdentity(aa_get_obj_tpname(this));
//	}
//	bool ret = taskExecuter_->bindScheduler(taskScheduler);
//	LOGFMTI("bindTaskScheduler() classname: %s, ret = %d", aa_get_obj_tpname(this).c_str(), ret);
//	return ret;
//}
//
//TaskScheduler* LogicModuleBase::getBindTaskScheduler()
//{
//	if (taskExecuter_ == nullptr) {
//		return nullptr;
//	}
//	return taskExecuter_->getBindScheduler();
//}
//
//std::shared_ptr<TaskExecuter> LogicModuleBase::getTaskExecuter()
//{
//	return taskExecuter_;
//}
//
//bool LogicModuleBase::sendModuleAsyncRequest(std::shared_ptr<AsyncPeer>& asyncPeer, AsyncPeer::AckCB ackCB, const int delayTimeMs /*= 0*/)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("sendModuleAsyncRequest() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	return taskExecuter_->sendAsyncRequest(asyncPeer, ackCB, delayTimeMs);
//}
//
//bool LogicModuleBase::sendModuleAsyncRequest(std::shared_ptr<AsyncPeer>& asyncPeer, const int delayTimeMs /*= 0*/)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("sendModuleAsyncRequest() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	return taskExecuter_->sendAsyncRequest(asyncPeer, delayTimeMs);
//}
//
//bool LogicModuleBase::bindModuleAsyncProcCB(const int64_t cmdId, ExecuterAsyncProcCB asyncCB)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("bindModuleAsyncProcCB() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	return taskExecuter_->bindCmdAsyncProcCB(cmdId, asyncCB);
//}
//
//bool LogicModuleBase::sendModuleAsyncResult(std::shared_ptr<AsyncPeer> asyncPeer, AsyncPeer::AckCB ackCB /*= nullptr*/)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("sendModuleAsyncResult() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	return taskExecuter_->sendAsyncResult(asyncPeer, ackCB);
//}
//
//bool LogicModuleBase::runAsyncTaskInMain(std::function<void()> asyncCB, const int delayTimeMs /*= 0*/)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("runAsyncTaskInMain() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	return taskExecuter_->postAsyncTaskToMain(asyncCB, delayTimeMs);
//}
//
//bool LogicModuleBase::runAsyncTaskInChild(std::function<void()> asyncCB, const int delayTimeMs /*= 0*/)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("runAsyncTaskInChild() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		return false;
//	}
//	bool ret = false;
//	// 在调度线程中调用
//	if (taskExecuter_->getSchedThreadId() == AASystemApi::getCurrentThreadId()) {
//		ret = taskExecuter_->postInnnerAsyncTask(asyncCB, delayTimeMs);
//	}
//	else {
//		ret = taskExecuter_->postAsyncTask(asyncCB, delayTimeMs);
//	}
//	return ret;
//}
//
//int64_t LogicModuleBase::runAsyncTaskBlocked(std::function<int64_t()> blockTaskCB)
//{
//	if (taskExecuter_ == nullptr) {
//		LOGFMTE("runAsyncTaskBlocked() Must initModule First!! taskExecuter == nullptr, classname: %s",
//			aa_get_obj_tpname(this).c_str());
//		throw std::runtime_error("taskExecuter_ == nullptr");
//	}
//	int64_t errCode = 0;
//	if (taskExecuter_->getSchedThreadId() == AASystemApi::getCurrentThreadId()) {
//		// 若在调度线程中调用,则直接执行任务,避免等待导致调度线程死锁
//		errCode = blockTaskCB();
//	}
//	else {
//		// 若在非调度线程中调用,则以异步阻塞的方式执行任务,避免同步和异步执行任务时出现数据竞争
//		// 同时可增加多个线程同时调用接口时的请求吞吐率
//		std::promise<int64_t> prom;
//		auto f = prom.get_future();
//		taskExecuter_->postAsyncTask(
//			[&prom, &blockTaskCB] {
//				int64_t errCode = blockTaskCB();
//				prom.set_value(errCode);
//			});
//		errCode = f.get();
//	}
//	return errCode;
//}
//ZCORE_NS_END
