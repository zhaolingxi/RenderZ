#pragma once
#include "zutils_sdk.h"
#include"zstring.h"
#include "json.h"
ZUTILS_NS_BEGIN
class ZUTILS_API ZJsonUtils
{
public:
	/**
	 @brief 将Json对象转换成字符串
	 @param[in] jsonValue JSON对象
	 @return Json对象对于的字符串
	 */
	static std::shared_ptr<ZString> jsonToStr(const Json::Value& jsonValue);
};
ZUTILS_NS_END