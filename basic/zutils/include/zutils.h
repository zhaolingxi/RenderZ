#pragma once
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4819)
#endif
#include "zutils_sdk.h"

#if defined(WIN32) || defined(WIN64)
// 仅支持使用在windows上的接口
#else
// 仅支持使用在linux上的接口
#endif
// 基础模板库
ZUTILS_NS_BEGIN
ZUTILS_NS_END
using namespace ZUtilsNS;
