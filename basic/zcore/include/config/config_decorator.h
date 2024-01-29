#pragma once
#include "zcore_sdk.h"
#include "config/config_base.h"
#include <memory>
/**********************************************************************
 
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API ConfigDecorator: public ConfigBase 
{
public:
	ConfigDecorator(std::shared_ptr<ConfigBase> config);
	virtual ~ConfigDecorator();
public:
	bool getValueBool(const char* key, const bool defaultVal = false) override;
	
    int getValueInt(const char* key, const int defaultVal = 0) override;

    double getValueDouble(const char* key, const double defaultVal = 0) override;

    ZSpString getValueString(const char* key, const char* defaultVal = "") override;

    ZSpVecInt getValueInts(const char* key) override;

    ZSpVecDouble getValueDobules(const char* key) override;

    ZSpVecStdString getValueStrings(const char* key) override;

	/**
 	  @brief 获取Json配置项的值
 	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
 	  @return 配置项对应的Json对象
 	 */
	const Json::Value* getJsonValue(const char* key) override;

    void parseConfig() override;

protected:
    std::shared_ptr<ConfigBase> getConfigInst();

private:
    std::weak_ptr<ConfigBase> configInst_; /**< 配置项实例 */
};
ZCORE_NS_END