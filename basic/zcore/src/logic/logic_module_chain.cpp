//#include "logic/logic_module_chain.h"
//#include "log4z.h"
//ZCORE_NS_BEGIN
//void LogicModuleChain::setModuleIdentity(int64_t moduleId)
//{
//	logicModIdentity_ = moduleId;
//}
//
//const int64_t LogicModuleChain::getModuleIdentity() const
//{
//	return logicModIdentity_;
//}
//
//void LogicModuleChain::addDependModuleId(const int64_t baseModuleId)
//{
//	dependBaseIdentitys_.push_back(baseModuleId);
//}
//
//int LogicModuleChain::getDependModuleCnt()
//{
//	return (int)dependBaseIdentitys_.size();
//}
//
//const int64_t LogicModuleChain::getDependModuleId(const int idx)
//{
//	if (idx < 0 || idx >= dependBaseIdentitys_.size()) {
//		LOGFMTE("getDependModuleId() invalid idx = %d", idx);
//		throw std::runtime_error("getDependModuleId() invalid idx");
//	}
//	return dependBaseIdentitys_[idx];
//}
//ZCORE_NS_END
