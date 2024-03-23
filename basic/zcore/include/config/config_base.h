#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include <memory>
#include"config_define.h"
#include"zutils.h"
#include"zstring.h"
#include<unordered_map>
/**********************************************************************
 *@brief  模块配置基础接口
 ***********************************************************************/
ZCORE_NS_BEGIN
class ConfigManager;
class ZCORE_API ConfigBase
{
public:
	using ConfigPtr = std::shared_ptr<ConfigBase>;
public:
	ConfigBase() {};
	ConfigBase(ConfigManager*& cfgMgr);
	virtual ~ConfigBase();

public:
	template<typename T>
	bool getConfigData(const ConfigType& configType, const ConfigFileType& configFileType, const DataType& dataType, const char* key,T &data);

public:
	int getValueInt(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const int defaultVal = 0);

	long long getValueInt64(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const long long defaultVal = 0);

	double getValueDouble(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const double defaultVal = 0);

	std::shared_ptr<ZString> getValueString(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const char* defaultVal = "");

	bool getValueBool(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const bool defaultVal = false);

	ZSpVecInt getValueInts(const ConfigType& configType, const ConfigFileType& configFileType);

	ZSpVecLL getValueInt64s(const ConfigType& configType, const ConfigFileType& configFileType);

	ZSpVecDouble getValueDobules(const ConfigType& configType, const ConfigFileType& configFileType);

	//virtual ZSpVecZString getValueStrings(const ConfigType& configType, const ConfigFileType& configFileType);

	void parseConfig(const ConfigFileType& configFileType);

private:
	ConfigManager* cfgManager_{ nullptr }; /**< 配置管理器 */
	//std::unordered_map<ZString, ConfigNode> configCache_;
	//std::unordered_map<std::string, ConfigNode> configCache_;
};


ZCORE_NS_END