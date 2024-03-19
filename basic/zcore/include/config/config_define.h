#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include "zstring.h"
ZCORE_NS_BEGIN
enum class ConfigType :int {
	GUI = 0,
	NETWORK,
	DATABASE,
	SYSTEAM,
	USER,
};

enum class ConfigFileType :int {
	JSON = 0,
	YAML,
	INI,
	XML,
	USER,
};

enum class DataType :int {
	INT32 = 0,
	INT64,
	FLOAT,
	DOUBLE,
	LONGLONG,
	BOOL,
	STRING,
	USER,
};


union ConfigDataValue
{
	int iVal;
	float fVal;
	int64_t i64TVal;
	int32_t i32TVal;
	std::shared_ptr<ZString> strVal;
};

struct ConfigNode
{
	DataType type;
	ConfigDataValue val;
};
ZCORE_NS_END