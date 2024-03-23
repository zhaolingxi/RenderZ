#include"config/config_base.h"
ZCORE_NS_BEGIN
ConfigBase::ConfigBase(ConfigManager*& cfgMgr)
{
	cfgManager_ = cfgMgr;
}

ConfigBase::~ConfigBase()
{

}

int ConfigBase::getValueInt(const ConfigType& configType,
	const ConfigFileType& configFileType, const char* key, const int defaultVal)
{
	return defaultVal;
}

long long ConfigBase::getValueInt64(const ConfigType& configType, 
	const ConfigFileType& configFileType, const char* key, const long long defaultVal)
{
	return defaultVal;
}

double ConfigBase::getValueDouble(const ConfigType& configType, 
	const ConfigFileType& configFileType, const char* key, const double defaultVal)
{
	return defaultVal;
}

std::shared_ptr<ZString> getValueString(const ConfigType& configType,
	const ConfigFileType& configFileType, const char* key, const char* defaultVal = "")
{
	return std::make_shared<ZString>(defaultVal);
}


ZSpVecInt getValueInts(const ConfigType& configType, const ConfigFileType& configFileType)
{
	return std::make_shared<ZVecInt>();
}

ZSpVecLL getValueInt64s(const ConfigType& configType, const ConfigFileType& configFileType)
{
	return std::make_shared<ZVecLL>();
}

ZSpVecDouble getValueDobules(const ConfigType& configType, const ConfigFileType& configFileType)
{
	return std::make_shared<ZVecDouble>();
}

//ZSpVecZString getValueStrings(const ConfigType& configType, const ConfigFileType& configFileType)
//{
//	return std::make_shared<ZVecDouble>();
//}


template<typename T>
inline bool ConfigBase::getConfigData(const ConfigType& configType, const ConfigFileType& configFileType, const DataType& dataType, const char* key, T& data)
{
	switch (dataType)
	{
		case DataType::INT32{
			data=getValueInt(configType,configFileType,key);
			break;
		}
		case DataType::INT64{
			data = getValueInt64(configType,configFileType,key);
			break;
		}
		case DataType::FLOAT{
			break;
		}
		case DataType::DOUBLE{
			data = getValueDouble(configType,configFileType,key);
			break;
		}
		case DataType::LONGLONG{
			break;
		}
		case DataType::STRING{
			break;
		}
		case DataType::BOOL{
			break;
		}
		case DataType::USER{
			break;
		}
		default:
			break;
	}
	return false;
}

ZCORE_NS_END