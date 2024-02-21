#pragma once
#include "zcore_sdk.h"
#include "zutils_define.h"
#include <memory>
#include "json/value.h"
/**********************************************************************
 *@brief  模块配置基础接口
 ***********************************************************************/
ZCORE_NS_BEGIN
class ConfigManager;
class CoreConfig;
class DatabaseConfig;
class GUIConfig;
class NetworkConfig;
class ZCORE_API ConfigBase
{
public:
	using ConfigPtr = std::shared_ptr<ConfigBase>;
public:
	ConfigBase() = default;
	ConfigBase(ConfigManager* cfgMgr);
	virtual ~ConfigBase();

public:
	// aacore
	// 业务插件目录和加载的插件名称
	std::shared_ptr<CoreConfig> getCoreConfig();

	// aagui
	// GUI窗体配置
	std::shared_ptr<GUIConfig> getGUIConfig();

	// aanetwork
	// 网络配置
	std::shared_ptr<NetworkConfig> getNetworkConfig();

public:
	/**
	  @brief 获取BOOL类型配置项的取值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @param[in] defaultVal 配置项的默认值
	  @return 配置项的取值
	 */
	virtual bool getValueBool(const char* key, const bool defaultVal = false);

	/**
	  @brief 获取整型配置项的取值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @param[in] defaultVal 配置项的默认值
	  @return 配置项的取值
	 */
	virtual int getValueInt(const char* key, const int defaultVal = 0);

	/**
	  @brief 获取整型配置项的取值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @param[in] defaultVal 配置项的默认值
	  @return 配置项的取值
	 */
	virtual long long getValueInt64(const char* key, const long long defaultVal = 0);

	/**
	  @brief 获取浮动型配置项的取值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @param[in] defaultVal 配置项的默认值
	  @return 配置项的取值
	 */
	virtual double getValueDouble(const char* key, const double defaultVal = 0);

	/**
	  @brief 获取字符型配置项的值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @param[in] defaultVal 配置项的默认值
	  @return 配置项的取值,指向字符串的 shared_ptr
	 */
	virtual std::shared_ptr<std::string> getValueString(const char* key, const char* defaultVal = "");

	/**
	  @brief 获取整型数组配置项的值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @return 数组配置项的值 shared_ptr
	 */
	virtual ZSpVecInt getValueInts(const char* key);

	/**
      @brief 获取整型数组配置项的值
      @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
      @return 数组配置项的值 shared_ptr
     */
	virtual ZSpVecLL getValueInt64s(const char* key);

	/**
	  @brief 获取浮动型数组配置项的值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @return 数组配置项的值 shared_ptr
	 */
	virtual ZSpVecDouble getValueDobules(const char* key);

	/**
	  @brief 获取字符串数组配置项的值
	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
	  @return 数组配置项的值 shared_ptr
	 */
	virtual ZSpVecStdString getValueStrings(const char* key);

	/**
 	  @brief 获取Json配置项的值
 	  @param[in] key 配置项的键值, 使用 / 表示配置项的层级关系; 如: driver_view/freq
 	  @return 配置项对应的Json对象
 	 */
	virtual const Json::Value *getJsonValue(const char* key);

	/**
	  @brief 解析模块的配置信息,用于实现模块配置项的差异化处理，将配置信息保存到缓存
	  @return 无
	 */
	virtual void parseConfig();

private:
	ConfigManager* cfgManager_{ nullptr }; /**< 配置管理器 */
};
ZCORE_NS_END
