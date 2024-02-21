#pragma once
#include "zutils_sdk.h"
#include "zmisc_utils.hpp"
#include "zutils_define.h"
#include "zstring.h"
#include <vector>
#include <memory>
/**********************************************************************
 *@brief 文件夹和文件夹路径操作接口
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZPathUtils
{
public:
	/**
	  @brief 将带有标签的路径,解析成实际的路径
	  @param[in] tagPath 带有标签的路径
	  @param[out] realPath 解析完标签后的实际路径
	  @return 无
	 */
	static void parsePath(const ZString& tagPath, ZString& realPath);

	/**
	  @brief 将带有标签的路径,解析成实际的路径
	  @param[in] tagPath 带有标签的路径
	  @return 解析完标签后的实际路径
	 */
	static  std::shared_ptr<std::string> parsePath(const ZString& tagPath);

	/**
	  @brief 连接路径
	  @param[in] dirPath 文件夹路径 
	  @param[in] childPath 文件夹下的子文件夹
	  @param[out] combPath 连接后的路径
	  @return 无
	 */
	static void joinPath(const ZString &dirPath, const ZString &childPath, ZString& combPath);

	/**
	  @brief 获取路径中最里层的文件夹或文件名称
	  @param[in] filePath 文件路径
	  @param[out] strName 最里层的文件夹或文件名称
	  @return 无
	 */
	static void baseName(const ZString& filePath, ZString& strName);

	/**
 	  @brief 获取路径中去除最里层文件或文件夹名称后的路径
 	  @param[in] filePath 文件路径
	  @param[out] strPath 去除最里层文件或文件夹名称后的路径
 	  @return  无
 	 */
	static void dirPath(const ZString& filePath, ZString& strPath);

	/**
 	  @brief 连接路径
 	  @param[in] dirPath 文件夹路径
 	  @param[in] childPath 文件夹下的子文件夹
 	  @return 连接后的路径
 	 */
	static std::shared_ptr<std::string> joinPath(const ZString& dirPath, const ZString& childPath);

	/**
	  @brief 获取路径中最里层的文件夹或文件名称
	  @param[in] filePath 文件路径
	  @return 最里层的文件夹或文件名称
	 */
	static std::shared_ptr<std::string> baseName(const ZString& filePath);

	/**
 	  @brief 获取路径中去除最里层文件或文件夹名称后的路径
 	  @param[in] filePath 文件路径
 	  @return 去除最里层文件或文件夹名称后的路径
 	 */
	static std::shared_ptr<std::string> dirPath(const ZString& filePath);
};
ZUTILS_NS_END