#pragma once
#include"zutils_sdk.h"
#include <vector>
#include <memory>

ZUTILS_NS_BEGIN

struct ZFileInfo
{
	//AAString filePath{ "" }; /**< 相对父文件夹的带路径的文件/文件夹名称 */
	//AAString fileName{ "" }; /**< 文件夹/文件名称 */
	//AAString fileExt{ "" };  /**< 文件后缀名,如: .txt, .json */
	//bool isDir{ false }; /**< 是否是文件夹 */
	//time_t createTime{ 0 }; /**< 文件创建时间，距离1970-01-01 0:0:0的秒数 */
};

using ZFileInfoVector = std::vector<ZFileInfo>;
using ZFileInfoVectorPtr = std::shared_ptr<ZFileInfoVector>;

class ZUTILS_API ZFileUtils
{
public:
	/**
	 @brief 设置文件权限掩码
	 @param[in] val: 文件权限掩码,所有创建的文件权限为: (val & ~umask & 0777)
	 @return true/false
	 */
	static void setUnmask(const int val);

	/**
	  @brief 获取文件大小
	  @param[in] filePath 带路径的文件名称
	  @return 文件大小
	 */
	static size_t getFileSize(const char* filePath);

	/**
	 @brief 文件是否存在
	 @param[in] fileName: 带路径的文件名
	 @return true/false
	 */
	static bool isFileExit(const char* fileName);

	/**
	 @brief 删除文件
	 @param[in] fileName: 带路径的文件名
	 @return true/false
	 */
	static bool deleteFile(const char* fileName);

	/**
	  @brief 拷贝文件
	  @param[in] srcFilePath 源文件路径
	  @param[in] dstFilePath 目的文件路径
	  @return true/false
	 */
	static bool copyFile(const char* srcFilePath, const char* dstFilePath);

	/**
	 @brief 文件夹是否存在
	 @param[in] dirName 带路径的文件夹名称
	 @return true/false
	 */
	static bool isDirExist(const char* dirName);

	/**
	 @brief 创建文件夹
	 @param[in] dirName 带路径的文件夹名称
	 @param[in] mode 文件夹权限
	 @return true/false
	 */
	static bool createDir(const char* dirName, const int mode = 0755);

	/**
	 @brief 递归创建文件夹和所有子文件夹
	 @param[in] dirName 带路径的文件夹名称
	 @return true/false
	 */
	static bool createDirs(const char* dirName);
	/**
	 @brief 删除文件夹
	 @param[in] dirName 带路径的文件夹名称
	 @return true/false
	 */
	static bool deleteDir(const char* dirName);

	/**
	 @brief 递归删除文件夹和所有子文件夹
	 @param[in] dirName 带路径的文件夹名称
	 @return true/false
	 */
	static bool deleteDirs(const char* dirName);

	/**
	 @brief 获取文件夹中的所有文件和子文件夹
	 @note pattern 参数在linux下只能传nullptr, windows下可以支持按后缀匹配:如:*.txt
	 @param[in]  dirName   带路径的文件夹名称
	 @param[out] fileInfos 保存子文件和子文件夹信息的缓存
	 @param[in]  maxCnt    缓存可保存的最大文件信息数量
	 @param[out] totalCnt  文件夹包含的子文件和子文件夹总数量
	 @param[in]  pattern   文件匹配选项,默认匹配所有文件
	 @return 失败: -1, 成功:文件夹包含的子文件和子文件夹数量
	 */
	static int getFilesInDir(const char* dirName, ZFileInfo* fileInfos, const int maxCnt, int* totalCnt, const char* pattern = nullptr);



protected:
	/**
	 @brief 递归获取文件夹中的所有文件和子文件夹,子文件夹中的文件和文件夹....,直到遍历完所有文件夹为止
	 @note pattern 参数在linux下只能传nullptr, windows下可以支持按后缀匹配:如:*.txt
	 @param[in]  dirName   带路径的文件夹名称
	 @param[out] fileInfos 文件夹下的文件和子文件夹信息
	 @param[in]  pattern   文件匹配选项,默认匹配所有文件
	 @return true/false
	 */
	static bool getFilesInDirsImpl(const char* dirName, ZFileInfoVector& fileInfos, const char* pattern);

	/**
	 @brief 获取文件夹下的文件和子文件夹
	 @note pattern 参数在linux下只能传nullptr, windows下可以支持按后缀匹配:如:*.txt
	 @param[in]  dirName   带路径的文件夹名称
	 @param[out] fileInfos 文件夹下的文件和子文件夹信息
	 @param[in]  pattern   文件匹配选项,默认匹配所有文件
	 @return true/false
	 */
	static bool getFilesInDirImpl(const char* dirName, ZFileInfoVector& fileInfos, const char* pattern);
};

ZUTILS_NS_END