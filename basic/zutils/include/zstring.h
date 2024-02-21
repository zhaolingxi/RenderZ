#pragma once
#include "zutils_sdk.h"
#include <vector>
#include <string>
#include <memory>
#include"zutils_define.h"
/**********************************************************************
 *@file  
 *@brief  字符串处理接口
		  1. 避免动态库导出警告和标准string跨模块回收导致程序崩溃
		  2. 针对长度<=DEFAULT_LEN的字符串，直接使用栈空间，减少内存申请和释放
 ***********************************************************************/
ZUTILS_NS_BEGIN
class ZUTILS_API ZString
{
public:
	static constexpr int DEFAULT_LEN = 256; /**< 字符串的默认大小 */
public:
	explicit ZString();

	/**
	 @brief 取消explicit限制,允许使用下面的方式构造ZString
			ZString str = "abc";
	 */
	ZString(const char* str);

	/**
	 @brief 允许使用下面的方式构造ZString
			ZString str("abc", 3);
	 */
	explicit ZString(const char* str, const int len);

	/**
	 @brief 取消explicit限制,允许使用下面的方式构造ZString
			std::string str = "abc";
			ZString str2 = str;
	 */
	ZString(const std::string& str);

	/**
	 @brief  取消explicit限制,允许将ZString作为返回值,或使用下面的方式构造ZString
			 ZString str1 = "abcd";
			 ZString str2 = str1;
	 */
	ZString(const ZString& str);

	/**
	 @brief 右值拷贝,允许使用下面的方式构造ZString,不会重新构造空间
			ZString str1 = "abc";
			ZString str3 = std::move(str1);
	 */
	ZString(ZString&& str);
	virtual ~ZString();
public:

	/**
	  @brief 将字符串中的srcCh替换为dstCh
	  @param[in] srcCh 被替换的字符
	  @param[in] dstCh 需要替换成的目标字符
	  @return 无
	 */
	void replace(const char srcCh, const char dstCh);

	/**
	 @brief 设置字符串的字符串(会改变字符串的长度,即: getLength的返回值)
	 @param[in] idx 索引
	 @param[in] ch 字符值
	 @return 无
	 */
	void setAt(const int idx, const char ch);
	/**
	 @brief 获取字符串缓存首地址
	 @return 字符串缓存首地址
	 */
	const char* getData() const;

	/**
	 @brief 获取字符串长度
	 @return 字符串长度
	 */
	const int getLength() const;

	/**
	  @brief 判断字符串是否为空
	  @return true/false
	 */
	const bool isEmpty() const;

	/**
	 @brief 获取字符串缓存大小(最大可保存多少个字符,包括\0)
	 @return 字符串缓存大小
	 */
	const int getBufferSize() const;

	/**
	 @brief 比较ZString与str(区分大小写)
	 @param[in] str 待比较的字符串
	 @param[in] len 代码比较字符串的长度
	 @return ZString==str: 返回0
			 ZString > str: 返回值大于0
			 ZString < str : 返回值小于0
	 */
	int compare(const char* str, const int len = -1) const;

	/**
	 @brief 比较ZString与str(不区分大小写)
	 @param[in] str 待比较的字符串
	 @param[in] len 代码比较字符串的长度
	 @return ZString==str: 返回0
			 ZString > str: 返回值大于0
			 ZString < str : 返回值小于0
	 */
	int compareNoCase(const char* str, const int len = -1) const;

	/**
	 @brief 判断ZString是否包含指定的子串(区分大小写)
	 @param[in] str 子串
	 @param[in] len 子串的大小
	 @return true/false
	 */
	bool contain(const char* str, const int len = -1) const;

	/**
	 @brief 判断ZString是否包含指定的子串(不区分大小写)
	 @param[in] str 子串
	 @param[in] len 子串的大小
	 @return true/false
	 */
	bool containNoCase(const char* str, const int len = -1) const;

	/**
	 @brief 设置字符串的值(会重置字符串缓存)
	 @param[in] str 字符串
	 @param[in] len 字符串的长度
	 @return 无
	 */
	void assign(const char* str, const int len = -1);

	/**
	 @brief 将字符串转换为大写
	 @return 大写字符串
	 */
	const char* toUpper();

	/**
	 @brief 将字符串转换为小写
	 @return 小写字符串
	 */
	const char* toLower();

	/**
	 @brief 求字符串的子串
	 @param[in] pos 子串的起始位置
	 @param[in] count 子串中包含的字符数量
	 @return 子串
	 */
	const ZString subString(const int pos, const int count = -1) const;

	/**
	 @brief 正向查找指定字符串
	 @param[in] strFind 待查找的字符串
	 @param[in] pos  查找的起始位置
	 @return 成功: 字符串第一次出现的位置, 失败：-1
	 */
	int  find(const char* strFind, const int pos = 0);

	/**
	 @brief 正向查找指定字符
	 @param[in] ch   待查找的字符
	 @param[in] pos  查找的起始位置
	 @return 成功: 字符第一次出现的位置, 失败：-1
	 */
	int  find(const char ch, const int pos = 0);

	/**
	 @brief 反向查找指定字符串
	 @param[in] strFind 待查找的字符串
	 @param[in] pos  查找的起始位置
	 @return 成功: 字符串第一次出现的位置, 失败：-1
	 */
	int  reverseFind(const char* strFind, const int pos = -1);

	/**
	 @brief 反向查找指定字符
	 @param[in] ch  待查找的字符
	 @param[in] pos 查找的起始位置
	 @return 成功: 字符第一次出现的位置, 失败：-1
	 */
	int  reverseFind(const char ch, const int pos = -1);

	/**
 	  @brief 判断字符串是否以指定字符串开头(区分大小写)
 	  @param[in] str 待比较字符串
	  @param[in] len 待比较字符串的长度
 	  @return true/false
 	 */
	bool beginWith(const char* str, const int len = -1) const;

	/**
 	  @brief 判断字符串是否以指定字符串开头(不区分大小写)
 	  @param[in] str 待比较字符串
	  @param[in] len 待比较字符串的长度
 	  @return true/false
 	 */
	bool beginWithNoCase(const char* str, const int len = -1);

	/**
 	  @brief 判断字符串是否以指定字符串结尾(区分大小写)
 	  @param[in] str 待比较字符串
	  @param[in] len 待比较字符串的长度
 	  @return true/false
 	 */
	bool endWith(const char* str, const int len = -1);

	/**
 	  @brief 判断字符串是否以指定字符串结尾(不区分大小写)
 	  @param[in] str 待比较字符串
	  @param[in] len 待比较字符串的长度
 	  @return true/false
 	 */
	bool endWithNoCase(const char* str, const int len = -1);

	/**
	  @brief 判断字符串是否以指定字符结尾
	  @param[in] ch 给定的字符
	  @return true/false
	 */
	bool endWith(const char ch);

	/**
	 @brief 追加字符串到String末尾
	 @param[in] str 待追加的字符串
	 @param[in] len 大追加字符串的长度
	 @return 无
	 */
	void append(const char* str, const int len = -1);

	/**
	 @brief 追加字符到String末尾
	 @param[in] ch 待追加的字符
	 @return 无
	 */
	void append(const char ch);

	/*
	 @brief 将字符串的值设置为格式化串的结果(使用方式类似与sprintf)
	 @note  特别提醒: 请注意格式化符的使用
			int/int32_t                 -- %d
			unsigned int/uint32_t       -- %u
			long                        -- %ld
			unsigned long               -- %lu
			long long/int64_t           -- %lld
			unsigned long long/uint64_t -- %llu
	 @param[in] format 字符串格式
	 @param[in] ... 与格式化对应的内容
	 @return 无
	*/
	void format(const char* format, ...);

	/**
	 @brief 追加格式化字符串(使用方式类似与sprintf)
	 @note  特别提醒: 请注意格式化符的使用
			int/int32_t                 -- %d
			unsigned int/uint32_t       -- %u
			long                        -- %ld
			unsigned long               -- %lu
			long long/int64_t           -- %lld
			unsigned long long/uint64_t -- %llu
	 @param[in] format 字符串格式
	 @param[in] ... 与格式化对应的内容
	 @return 无
	 */
	void appendFormat(const char* format, ...);

	/**
	 @brief 清除字符串的左边和右边的空白字符(包括空格、\r、\n、\t)
	 @return 无
	 */
	void trim();
	/**
	 @brief 清除字符串的左边的空白字符(包括空格、\r、\n、\t)
	 @return 无
	 */
	void trimLeft();

	/**
	 @brief 清除字符串右边的空白字符(包括空格、\r、\n、\t)
	 @return 无
	 */
	void trimRight();

	/**
	 @brief 字符串转换成整数
	 */
	int toInt() const;
	int64_t toInt64() const;

	/*
	 @brief 将字符串转换为小数
	*/
	float toFloat() const;
	double toDouble() const;
public:
	const char operator[](int idx) const;
	// 会重置字符串缓存
	const ZString& operator=(const ZString& src);
	// 会重置字符串缓存
	const ZString& operator=(ZString&& src);
	// 会重置字符串缓存
	const ZString& operator=(const char ch);
	// 会重置字符串缓存
	const ZString& operator=(const char* str);
	// 会重置字符串缓存
	const ZString& operator=(const std::string& str);
	ZString operator+(const ZString& src);
	ZString operator+(const char* str);
	const ZString& operator+=(const ZString& src);
	const ZString& operator+=(const char* str);
	const ZString& operator+=(const char ch);

	const bool operator ==(const ZString& str) const;
	const bool operator !=(const ZString& other) const;
	const bool operator >(const ZString& other) const;
	// 适配 map、set等容器
	const bool operator <(const ZString& other) const;
	// 字符串的hash值
	size_t hashCode() const noexcept;

public:
	/**
	 @brief 给定的指针，指向的串是否是一个空串
	 @param[in] str 给定的字符串
	 @return true/false
	 */
	static bool isEmptyStr(const char* str);
	/**
	 @brief 根据指定字符拆分字符串
	 @param[in] str 源串
	 @param[in] ch  参考字符
	 @param[out] strList 拆分结果
	 */
	static ZSpVecStdString split(const ZString& strIn, char ch, bool isTrim = true);

	/**
	 @brief  删除字符串左边的空白字符(包括空格、\r、\n、\t)(strIn和strOut可以是同一对象)
	 @param[in] strIn 输入的字符串
	 @param[in] strOut 处理结果
	 */
	static void trimLeft(const ZString& strIn, ZString& strOut);
	/**
	 @brief  删除字符串右边的空白字符(包括空格、\r、\n、\t)(strIn和strOut可以是同一对象)
	 @param[in] strIn 输入的字符串
	 @param[in] strOut 处理结果
	 */
	static void trimRight(const ZString& strIn, ZString& strOut);
	/**
	 @brief  删除字符串左右两边的空白字符(包括空格、\r、\n、\t)(strIn和strOut可以是同一对象)
	 @param[in] strIn 输入的字符串
	 @param[in] strOut 处理结果
	 */
	static void trim(const ZString& strIn, ZString& strOut);

	static int toInt(const ZString& str);
	static int64_t toInt64(const ZString& str);
	static float toFloat(const ZString& str);
	static double toDouble(const ZString& str);

	/**
	 @brief 字符串是否是一个有效的整数串
	 @param[in] str 待判断的字符串
	 @return true/false
	 */
	static bool isValidInteger(const ZString& str);

	/**
	 @brief 字符串是否是一个有效的浮点数串
	 @param[in] str 待判断的字符串
	 @return true/false
	 */
	static bool isValidFloat(const ZString& str);

	/**
	 @brief 拷贝字符串(会确保内存是否越界并强制设置结束符)
	 @param[out] dstStr  保存字符串的目的缓存
	 @param[in]  dstSize 目的缓存的大小
	 @param[in]  srcStr  源串
	 @param[in]  srcLen  源串的长度
	 @return 无
	 */
	static void copyString(char* dstStr, const int dstSize, const char* srcStr, const int srcLen = -1);

	static void copyString(char* dstStr, const int dstSize, const ZString& srcStr);

	/**
	 @brief 判断主串中是否包含子串
	 @param[in] mainStr 指向主串的指针
	 @param[in] mainLen 主串的长度
	 @param[in] subStr  指向子串的指针
	 @param[in] subLen  子串的长度
	 @param[in] ingoreCase 是否忽略大小写
	 @return true/false
	 */
	static bool normalContain(const char* mainStr, const int mainLen, const char* subStr, const int subLen, bool ingoreCase = false);

	/**
	  @brief 字符串比较
	  @param[in] str1 字符串1
	  @param[in] str2 字符串2
	  @param[in] lenCompare 比较长度
	  @param[in] ingoreCase 是否忽略大小写
	  @return str1 > str2 返回值大于0
			  str1 < str2 返回值小于0
			  str1 == str2 返回值等于0
	 */
	static int compare(const char* str1, const char* str2, const int lenCompare, bool ingoreCase = false);

	static int compare(const std::string& str1, const std::string& str2, bool ingoreCase = false);

	static int compare(const ZString& str1, const ZString& str2, bool ingoreCase = false);

public:
	/**
	 @brief 确保字符串缓存足够保存指定长度的字符串
	 @param[in] len 字符串的长读
	 @return 无
	 */
	bool ensureBuffer(int len);

	/**
	  @brief 修改字符串的长度
	  @param[in] newLen 字符串长度
	  @return 无
	 */
	void setLength(const int newLen);

protected:
	/**
	 @brief 重置字符串缓存(释放字符串缓存空间)
	 @return 无
	 */
	void resetBuffer();
	/**
	 @brief 指定字符串缓存的值
	 @param[in] str 指向字符串的指针
	 @param[in] len 字符串的长度
	 @return 无
	 */
	void assignValue(const char* str, const int len);

private:
	char* strBuffer_{ nullptr }; /**< 保存字符串的缓存 */
	int bufferSize_{ 0 };  /**< 缓存大小 */
	int strLength_{ 0 };   /**< 字符串的长度 */
	char stackBuffer_[DEFAULT_LEN]; /**< 栈空间缓存(保存短字符串) */
	bool isStoreInHeapMem_{ false }; /**< 数据是否保存在堆内存中 */
};
ZUTILS_NS_END

// 适配unordered_map和unordered_set
namespace std {
	template<>
	class hash<ZUtilsNS::ZString>
	{
	public:
		size_t operator()(const ZUtilsNS::ZString& str) const {
			return str.hashCode();
		}
	};
}
