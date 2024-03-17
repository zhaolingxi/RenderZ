#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include <memory>
#include "json/value.h"
#include"config_define.h"
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
	ConfigBase() = default;
	//ConfigBase(ConfigManager* cfgMgr);
	//virtual ~ConfigBase();

public:
	template<typename T>
	bool getConfigData(const ConfigType& configType, const ConfigFileType& configFileType, const DataType& dataType, const char* key,T &data);

public:
	//virtual int getValueInt(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const int defaultVal = 0);

	//virtual long long getValueInt64(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const long long defaultVal = 0);

	//virtual double getValueDouble(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const double defaultVal = 0);

	//virtual std::shared_ptr<std::string> getValueString(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const char* defaultVal = "");

	//virtual bool getValueBool(const ConfigType& configType, const ConfigFileType& configFileType, const char* key, const bool defaultVal = false);

	//virtual ZSpVecInt getValueInts(const ConfigType& configType, const ConfigFileType& configFileType, const char* key);

	//virtual ZSpVecLL getValueInt64s(const ConfigType& configType, const ConfigFileType& configFileType, const char* key);

	//virtual ZSpVecDouble getValueDobules(const ConfigType& configType, const ConfigFileType& configFileType, const char* key);

	//virtual ZSpVecStdString getValueStrings(const ConfigType& configType, const ConfigFileType& configFileType, const char* key);

	//virtual void parseConfig(const ConfigFileType& configFileType);

private:
	ConfigManager* cfgManager_{ nullptr }; /**< 配置管理器 */
	//std::unordered_map<zutils::ZString, ConfigNode> configCache_;
	//std::unordered_map<std::string, ConfigNode> configCache_;
};


//template<typename T>
//inline bool ConfigBase::getConfigData(const ConfigType& configType, const ConfigFileType& configFileType, const DataType& dataType, const char* key, T& data)
//{
//	switch (dataType)
//	{
//		case DataType::INT32{
//		}
//		case DataType::INT64{
//		}
//		case DataType::FLOAT{
//		}
//		case DataType::DOUBLE{
//		}
//		case DataType::LONGLONG{
//		}
//		case DataType::STRING{
//		}
//		case DataType::BOOL{
//		}
//		case DataType::USER{
//		}
//	default:
//		break;
//	}
//	return false;
//}

ZCORE_NS_END