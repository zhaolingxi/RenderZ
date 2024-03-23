#pragma once
#include "zutils_sdk.h"
#include"zstring.h"
#include "json.h"
ZUTILS_NS_BEGIN
class ZUTILS_API ZJsonUtils
{
public:
	/**
	 @brief ��Json����ת�����ַ���
	 @param[in] jsonValue JSON����
	 @return Json������ڵ��ַ���
	 */
	static std::shared_ptr<ZString> jsonToStr(const Json::Value& jsonValue);
};
ZUTILS_NS_END