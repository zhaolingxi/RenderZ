#pragma once
#include "zcore_sdk.h"
#include "logic_module_chain.h"
#include <unordered_map>
#include <memory>
/**********************************************************************
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API LogicPluginModuleInfo
{
public:
	LogicPluginModuleInfo() = default;
	virtual ~LogicPluginModuleInfo() = default;
public:
	/**
	 @brief 添加插件中包含的业务模块
	 @param[in] moduleId 插件中包含业务模块的唯一标识
	 @return 无
	 */
	void addModuleIdentity(const int64_t moduleId);

	/**
	 @brief 添加Module依赖的子Module
	 @param[in] moduleId Module的唯一标识
	 @param[in] baseModuleId 依赖Module的唯一标识
	 @return  无
	 */
	void addDependModule(const int64_t moduleId, const int64_t baseModuleId);

	/**
	 @brief 获取插件中包含的Module数量
	 @return 插件中包含的Module数量
	 */
	int getModuleCnt();

	/**
	  @brief 获取插件中包含Module的唯一标识
	  @param[in] idxModule Module的顺序索引号
	  @return Module的唯一标识
	 */
	int64_t getModuleIdentity(const int idxModule);

	/**
	 @brief 获取Module的依赖链
	 @note 因为LogicModuleChain需要托管到LogicPluginManager
		   所以需要返回shared_ptr避免对象被重复构建
	 @param[in] moduleId Module的唯一标识
	 @return Module的依赖链
	 */
	std::shared_ptr<LogicModuleChain> getModuleChainByIdentity(const int64_t moduleId);

	/**
	 @brief 获取Module的依赖链
	 @note 因为LogicModuleChain需要托管到LogicPluginManager
		   所以需要返回shared_ptr避免对象被重复构建
	 @param[in] idxModule Module的顺序索引号
	 @return Module的依赖链
	 */
	std::shared_ptr<LogicModuleChain> getModuleChainByIdx(const int idxModule);

protected:
	/**
	 @brief 安全获取指定Module的依赖链,若存在则获取,若不存在则创建
	 @note 因为UIModuleChain需要托管到UIPluginManager
		   所以需要返回shared_ptr避免对象被重复构建
	 @param[in] ModuleId Module的唯一标识
	 @return Module的依赖链
	 */
	std::shared_ptr<LogicModuleChain>& ensureGetModuleChain(const int64_t ModuleId);

private:
	std::vector<int64_t> moduleIdentitys_; // 插件中包含的Module
	// Module的依赖Chain,存储Module依赖的子Module
	std::unordered_map<int64_t, std::shared_ptr<LogicModuleChain>> moduleChainMap_;
};
ZCORE_NS_END
