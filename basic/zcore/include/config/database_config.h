#pragma once
#include "zcore_sdk.h"
#include "config/config_decorator.h"
/**********************************************************************
 *@brief  数据库配置
 ***********************************************************************/
ZCORE_NS_BEGIN
class ZCORE_API DatabaseConfig : public ConfigDecorator
{
public:
	DatabaseConfig(std::shared_ptr<ConfigBase> config);
	virtual ~DatabaseConfig();
public:
	/**
	  @brief 获取主数据库文件名称
	  @return 主数据库文件名称
	 */
	ZSpString getMainSqliteFileName();
};
ZCORE_NS_END