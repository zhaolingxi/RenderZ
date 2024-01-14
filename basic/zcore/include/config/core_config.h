#pragma once
#include "zcore_sdk.h"
#include "config/config_decorator.h"
/**********************************************************************
 *@file   corecfg_decorator.h
 *@date   2023/01/16 10 : 34
 *@author shanwenbin
 *@brief  基础框架配置
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API CoreConfig : public ConfigDecorator
{
public:
	CoreConfig(std::shared_ptr<ConfigBase> config);
	virtual ~CoreConfig();
public:
	int getMainSingleLoopIntervalMS();
	int getMaxLoopTimeMS();
};
ZCORE_NS_END