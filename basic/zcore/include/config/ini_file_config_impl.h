#pragma once
#include "zcore_sdk.h"
#include "config/config_base.h"
#include <memory>
/**********************************************************************
 *@file   ini_file_config_impl.h
 *@date   2023/01/14 17 : 05
 *@author shanwenbin
 *@brief  使用ini文件保存的模块配置信息(待完善)
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API INIFileConfigImpl: public ConfigBase
{
public:
    INIFileConfigImpl() = default;
    virtual ~INIFileConfigImpl() = default;
public: 
	void setConfig(const char *iniFile);

public:
    int getValueInt(const char* key, const int defaultVal = 0) override;
    
    double getValueDouble(const char* key, const double defaultVal = 0) override;

    std::shared_ptr<std::string> getValueString(const char* key, const char* defaultVal = "") override;;

    AAIntVectorPtr getValueInts(const char* key) override;

    AADoubleVectorPtr getValueDobules(const char* key) override;

    AAStdStringVectorPtr getValueStrings(const char* key) override;
};
ZCORE_NS_END
