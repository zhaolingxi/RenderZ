#pragma once
#include "zutils_sdk.h"
#include <memory>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#else
typedef void* HINSTANCE;
#endif

// 公共类型定义
using ZVecStdString = std::vector<std::string>;
using ZVecInt = std::vector<int>;
using ZVecLL = std::vector<long long>;
using ZVecDouble = std::vector<double>;

using ZUSetString = std::unordered_set<std::string>;
using ZSetString = std::set<std::string>;

// 智能指针
using ZSpString = std::shared_ptr<std::string>;
using ZSpVecStdString = std::shared_ptr<ZVecStdString>;
using ZSpVecInt = std::shared_ptr<ZVecInt>;
using ZSpVecLL = std::shared_ptr<ZVecLL>;
using ZSpVecDouble = std::shared_ptr<ZVecDouble>;

ZUTILS_NS_BEGIN

ZUTILS_NS_END