#include "../include/zjson_utils.h"
ZUTILS_NS_BEGIN
std::shared_ptr<ZString> ZJsonUtils::jsonToStr(const Json::Value& jsonValue)
{
	std::shared_ptr<ZString> jsonStr=std::make_shared<ZString>();
	if (jsonValue.isNull()) {
		return jsonStr;
	}
	Json::FastWriter jsonWriter;
	*jsonStr = jsonWriter.write(jsonValue);
	return jsonStr;
}
ZUTILS_NS_END