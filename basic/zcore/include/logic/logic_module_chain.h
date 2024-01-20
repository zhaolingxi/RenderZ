#pragma once
#include "zcore_sdk.h"
#include <vector>
#include <cstdint>
/**********************************************************************
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API LogicModuleChain
{
public:
	LogicModuleChain() = default;
	virtual ~LogicModuleChain() = default;
public:
	/**
	 @brief 设置业务模块的唯一标识
	 @param[in] moduleId 业务模块的唯一标识
	 @return 无
	 */
	void setModuleIdentity(int64_t moduleId);
	const int64_t getModuleIdentity() const;

	/**
	 @brief 添加业务模块依赖的基础业务模块
	 @param[in] baseModuleId 基础业务模块的唯一标识
	 @return 无
	 */
	void addDependModuleId(const int64_t baseModuleId);

	/**
	 @brief 获取业务模块依赖的基础业务模块数量
	 @return 业务模块依赖的基础业务模块数量
	 */
	int getDependModuleCnt();
	const int64_t getDependModuleId(const int idx);

private:
	int64_t logicModIdentity_{ 0 }; // 业务模块的唯一标识
	std::vector<int64_t> dependBaseIdentitys_; // 业务模块依赖的基础模块的标识
};
ZCORE_NS_END
