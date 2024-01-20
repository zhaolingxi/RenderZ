#pragma once
#include "zcore_sdk.h"
#include "config/config_base.h"
#include "json/value.h"
#include <memory>
/**********************************************************************
 *@brief  使用JSON对象保存的模块配置信息
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API JsonConfigImpl: public ConfigBase
{
public:
	JsonConfigImpl(ConfigManager* cfgMgr);
	virtual ~JsonConfigImpl() = default;
public:
	/**
	  @brief 设置保存配置信息的Json对象
	  @param[in] jsonVal 保存配置信息的Json对象
	  @return 无
	 */
	void setConfig(const Json::Value &jsonVal);

public:
	bool getValueBool(const char* key, const bool defaultVal = false) override;

	int getValueInt(const char* key, const int defaultVal = 0) override;

	long long getValueInt64(const char* key, const long long defaultVal = 0) override;

	double getValueDouble(const char* key, const double defaultVal = 0) override;

	std::shared_ptr<std::string> getValueString(const char* key, const char* defaultVal = "") override;;

	AAIntVectorPtr getValueInts(const char* key) override;

	AAInt64VectorPtr getValueInt64s(const char* key) override;

	AADoubleVectorPtr getValueDobules(const char* key) override;

	AAStdStringVectorPtr getValueStrings(const char* key) override;
	
	const Json::Value* getJsonValue(const char* key) override;

protected:
	/**
	  @brief 获取配置项的Json对象
	  @param[in] itemNames 配置项的路径上对应的item的名称
	  @return 若存在则返回指向配置项的指针,若不存在则返回nullptr
	 */
	const Json::Value* getCfgJsonValue(const AAStdStringVector& itemNames);

private:
	Json::Value jsonCfg_{Json::nullValue}; /**< 保存配置信息的Json对象 */
};
ZCORE_NS_END
