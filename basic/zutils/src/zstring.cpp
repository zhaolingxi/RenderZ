#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <stdexcept>
#include <cstring>
#include "zstring.h"
#include "zmisc_utils.hpp"
#include "encode/zhash.h"
#include "zmem_utils.h"
ZUTILS_NS_BEGIN
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

ZString::ZString()
{
	isStoreInHeapMem_ = false;
	resetBuffer();
}

ZString::ZString(const char* str)
{
	isStoreInHeapMem_ = false;
	resetBuffer();
	if (!isEmptyStr(str)) {
		assignValue(str, (int)strlen(str));
	}
}

ZString::ZString(const char* str, const int len)
{
	isStoreInHeapMem_ = false;
	resetBuffer();
	if (!isEmptyStr(str)) {
		assignValue(str, len);
	}
}

ZString::ZString(const std::string& str)
{
	isStoreInHeapMem_ = false;
	resetBuffer();
	if (str.length() > 0) {
		assignValue(str.c_str(), (int)str.length());
	}
}

ZString::ZString(const ZString& str)
{
	isStoreInHeapMem_ = false;
	resetBuffer();
	if (str.getLength() > 0) {
		assignValue(str.getData(), str.getLength());
	}
}

ZString::ZString(ZString&& other)
{
	isStoreInHeapMem_ = other.isStoreInHeapMem_;
	if (other.isStoreInHeapMem_) {
		// 转接原字符串的缓存空间,避免大块空间多次分配
		strBuffer_ = other.strBuffer_;
		bufferSize_ = other.bufferSize_;
		strLength_ = other.strLength_;
	}
	else {
		// 转接原字符串控制权
		bufferSize_ = sizeof(stackBuffer_);
#ifdef _MSVC
		strncpy_s(stackBuffer_, bufferSize_, other.stackBuffer_, other.strLength_ + 1);
#else
		strncpy(stackBuffer_, other.stackBuffer_, other.strLength_ + 1);
#endif
		strBuffer_ = &stackBuffer_[0];
		strLength_ = other.strLength_;
	}
	// 重置原字符串
	other.isStoreInHeapMem_ = false;
	other.strLength_ = 0;
	other.strBuffer_ = &other.stackBuffer_[0];
	memset(&other.stackBuffer_[0], 0, sizeof(other.stackBuffer_));
}

ZString::~ZString()
{
	// 若有申请堆空间，则直接需要释放堆空间
	if (isStoreInHeapMem_) {
		ZSAFE_FREE(strBuffer_);
	}
}

void ZString::replace(const char srcCh, const char dstCh)
{
	for (int i = 0; i < strLength_; i++) {
		if (strBuffer_[i] == srcCh) {
			strBuffer_[i] = dstCh;
		}
	}
}

void ZString::setAt(const int idx, const char ch)
{
	if (idx < 0 || idx >= bufferSize_) {
		return;
	}
	strBuffer_[idx] = ch;
	// 调整字符串长度
	if (ch == (char)0 && idx <= strLength_ - 1) {
		strLength_ = idx;
	}
}

const char* ZString::getData() const
{
	return strBuffer_;
}

const int ZString::getLength() const
{
	return strLength_;
}

const bool ZString::isEmpty() const
{
	return (strLength_ <= 0);
}

const int ZString::getBufferSize() const
{
	return bufferSize_;
}

int ZString::compare(const char* str, const int len /*= -1*/) const
{
	if (str == nullptr) {
		return strLength_;
	}
	int tmpLen = len;
	if (tmpLen < 0) {
		tmpLen = (int)strlen(str);
	}
	if (tmpLen == 0) {
		return strLength_;
	}
	// 用结束符去进行最后一次比较
	int lenCompare = min(strLength_, tmpLen) + 1;
	int ret = strncmp(strBuffer_, str, lenCompare);
	return ret;
}

int ZString::compareNoCase(const char* str, const int len /*= -1*/) const
{
	if (str == nullptr) {
		return strLength_;
	}
	int tmpLen = len;
	if (tmpLen < 0) {
		tmpLen = (int)strlen(str);
	}
	if (tmpLen == 0) {
		return strLength_;
	}
	int lenCompare = min(strLength_, tmpLen) + 1;
#ifdef _MSVC
	int ret = _strnicmp(strBuffer_, str, lenCompare);
#else
	int ret = strncasecmp(strBuffer_, str, lenCompare);
#endif
	return ret;
}

bool ZString::contain(const char* str, const int len /*= -1*/) const
{
	if (str == nullptr) {
		return true;
	}
	int strLen = len;
	if (strLen < 0) {
		strLen = (int)strlen(str);
	}
	if (strLen == 0) {
		return true;
	}
	return normalContain(getData(), getLength(), str, strLen, false);
}

bool ZString::containNoCase(const char* str, const int len /*= -1*/) const
{
	if (str == nullptr) {
		return true;
	}
	int strLen = len;
	if (strLen < 0) {
		strLen = (int)strlen(str);
	}
	if (strLen == 0) {
		return true;
	}
	return normalContain(getData(), getLength(), str, strLen, true);
}

void ZString::assign(const char* str, const int len /*= -1*/)
{
	int strLen = len;
	resetBuffer();
	if (str == nullptr) {
		return;
	}
	if (strLen < 0) {
		strLen = strlen(str);
	}
	assignValue(str, strLen);
}

const char* ZString::toUpper()
{
	for (int i = 0; i < strLength_; i++) {
		if (strBuffer_[i] >= 'a' && strBuffer_[i] <= 'z') {
			strBuffer_[i] -= 32;
		}
	}
	return strBuffer_;
}

const char* ZString::toLower()
{
	for (int i = 0; i < strLength_; i++) {
		if (strBuffer_[i] >= 'A' && strBuffer_[i] <= 'Z') {
			strBuffer_[i] += 32;
		}
	}
	return strBuffer_;
}

const ZString ZString::subString(const int pos, const int count /*= -1*/) const
{
	int copyLen = 0;
	int maxLen = strLength_ - pos; // 子串的最大长度
	if (count < 0) {
		copyLen = maxLen;
	}
	else {
		copyLen = min(maxLen, count);
	}
	ZString tmpStr("");
	if (copyLen > 0) {
		tmpStr.assign(getData() + pos, copyLen);
	}
	return tmpStr;
}

int ZString::find(const char* strFind, const int pos /*= 0*/)
{
	if (strFind == nullptr) {
		return -1;
	}
	int posFind = -1;
	int len = (int)strlen(strFind);
	for (int i = pos; i + len <= strLength_; i++) {
		if (strncmp(strBuffer_ + i, strFind, len) == 0) {
			posFind = i;
			break;
		}
	}
	return posFind;
}

int ZString::find(const char ch, const int pos /*= 0*/)
{
	int posFind = -1;
	for (int i = pos; i < strLength_; i++) {
		if (strBuffer_[i] == ch) {
			posFind = i;
			break;
		}
	}
	return posFind;
}

int ZString::reverseFind(const char* strFind, const int pos /*= -1*/)
{
	if (strFind == nullptr) {
		return -1;
	}
	int posFind = -1;
	int len = (int)strlen(strFind);
	int startPos = pos;
	if (startPos < 0) {
		startPos = strLength_ - 1;
	}
	startPos = startPos - len + 1;
	for (int i = startPos; i >= 0; i--) {
		if (strncmp(strBuffer_ + i, strFind, len) == 0) {
			posFind = i;
			break;
		}
	}
	return posFind;
}

int ZString::reverseFind(const char ch, const int pos /*= -1*/)
{
	int posFind = -1;
	int startPos = pos;
	if (startPos < 0) {
		startPos = strLength_ - 1;
	}
	for (int i = startPos; i >= 0; i--) {
		if (strBuffer_[i] == ch) {
			posFind = i;
			break;
		}
	}
	return posFind;
}

bool ZString::beginWith(const char* str, const int len /*= -1*/) const
{
	if (ZString::isEmptyStr(str)) {
		return false;
	}
	int cnt = len;
	if (cnt < 0) {
		cnt = strlen(str);
	}
	if (strLength_ < cnt) {
		return false;
	}
	int ret = strncmp(strBuffer_, str, cnt);
	return (ret == 0);
}

bool ZString::beginWithNoCase(const char* str, const int len /*= -1*/)
{
	if (ZString::isEmptyStr(str)) {
		return false;
	}
	int cnt = len;
	if (cnt < 0) {
		cnt = strlen(str);
	}
	if (strLength_ < cnt) {
		return false;
	}
#ifdef _MSVC
	int ret = _strnicmp(strBuffer_, str, cnt);
#else
	int ret = strncasecmp(strBuffer_, str, cnt);
#endif
	return (ret == 0);
}

bool ZString::endWith(const char* str, const int len /*= -1*/)
{
	if (ZString::isEmptyStr(str)) {
		return false;
	}
	int cnt = len;
	if (cnt < 0) {
		cnt = strlen(str);
	}
	int pos = strLength_ - cnt;
	if (pos < 0) {
		return false;
	}
	int ret = strncmp(strBuffer_ + pos, str, cnt);
	return (ret == 0);
}

bool ZString::endWithNoCase(const char* str, const int len /*= -1*/)
{
	if (ZString::isEmptyStr(str)) {
		return false;
	}
	int cnt = len;
	if (cnt < 0) {
		cnt = strlen(str);
	}
	int pos = strLength_ - cnt;
	if (pos < 0) {
		return false;
	}
#ifdef _MSVC
	int ret = _strnicmp(strBuffer_ + pos, str, cnt);
#else
	int ret = strncasecmp(strBuffer_ + pos, str, cnt);
#endif
	return (ret == 0);
}

bool ZString::endWith(const char ch)
{
	if (strLength_ <= 0) {
		return false;
	}
	return (strBuffer_[strLength_ - 1] == ch);
}

const char ZString::operator[](int idx) const
{
	if (idx < 0 || idx >= strLength_) {
		throw std::runtime_error("idx < 0 || idx >= strLength_");
	}
	return strBuffer_[idx];
}

const ZString& ZString::operator=(const char* str)
{
	resetBuffer();
	if (str == nullptr) {
		return *this;
	}
	assignValue(str, (int)strlen(str));
	return *this;
}

const ZString& ZString::operator=(const char ch)
{
	resetBuffer();
	ensureBuffer(DEFAULT_LEN);
	strBuffer_[0] = ch;
	strBuffer_[1] = 0;
	return *this;
}

const ZString& ZString::operator=(const std::string& str)
{
	resetBuffer();
	ensureBuffer((int)str.length());
	assignValue(str.c_str(), (int)str.length());
	return *this;
}

const ZString& ZString::operator=(const ZString& src)
{
	resetBuffer();
	assignValue(src.getData(), src.getLength());
	return *this;
}

const ZString& ZString::operator=(ZString&& src)
{
	resetBuffer();
	isStoreInHeapMem_ = src.isStoreInHeapMem_;
	if (src.isStoreInHeapMem_) {
		// 转接原字符串的缓存空间,避免大块空间多次分配
		strBuffer_ = src.strBuffer_;
		bufferSize_ = src.bufferSize_;
		strLength_ = src.strLength_;
	}
	else {
		// 转接原字符串控制权,拷贝字符串时，需要多拷贝字符串的结束符 '\0'
#ifdef _MSVC
		strncpy_s(stackBuffer_, bufferSize_, src.stackBuffer_, src.strLength_ + 1);
#else
		strncpy(stackBuffer_, src.stackBuffer_, src.strLength_ + 1);
#endif
		strLength_ = src.strLength_;
	}
	// 重置原字符串
	src.isStoreInHeapMem_ = false;
	src.strLength_ = 0;
	src.strBuffer_ = &src.stackBuffer_[0];
	memset(&src.stackBuffer_[0], 0, sizeof(src.stackBuffer_));
	return *this;
}

ZString ZString::operator+(const ZString& src)
{
	ZString strTmp(*this);
	strTmp.append(src.getData(), src.getLength());
	return strTmp;
}

ZString ZString::operator+(const char* str)
{
	ZString strTmp(*this);
	strTmp.append(str, (int)strlen(str));
	return strTmp;
}

const ZString& ZString::operator+=(const ZString& src)
{
	append(src.getData(), src.getLength());
	return *this;
}

const ZString& ZString::operator+=(const char* str)
{
	append(str, (int)strlen(str));
	return *this;
}

const ZString& ZString::operator+=(const char ch)
{
	char str[] = { ch, 0 };
	append(str, 1);
	return *this;
}

const bool ZString::operator==(const ZString& other) const
{
	return compare(other.getData(), other.getLength()) == 0;
}

const bool ZString::operator!=(const ZString& other) const
{
	return compare(other.getData(), other.getLength()) != 0;
}

const bool ZString::operator>(const ZString& other) const
{
	return compare(other.getData(), other.getLength()) > 0;
}
// 适配map、set等容器
const bool ZString::operator<(const ZString& other) const
{
	return compare(other.getData(), other.getLength()) < 0;
}

// 确保字符串足够保留长度为len的字符串,需要预留保存结束符的位置
bool ZString::ensureBuffer(int len)
{
	if (bufferSize_ - strLength_ >= len + 1) {
		return true;
	}
	int newBufferSize = z_align8(bufferSize_ + len + 1) << 1; // 2倍扩容
	bufferSize_ = newBufferSize;
	char* newBuffer = nullptr;
	if (!isStoreInHeapMem_) { // 第一次申请堆空间
		newBuffer = (char*)calloc(1, bufferSize_);
		if (newBuffer == nullptr) {
			return false;
		}
		if (strLength_ > 0) {
			ZMemUtils::memCopy(newBuffer, bufferSize_, strBuffer_, strLength_);
		}
		isStoreInHeapMem_ = true;
	}
	else {
		newBuffer = (char*)realloc(strBuffer_, bufferSize_);
		if (newBuffer == nullptr) {
			return false;
		}
	}
	strBuffer_ = newBuffer; // 修改缓存指针
	return  true;
}


void ZString::setLength(const int newLen)
{
	if (newLen >= bufferSize_) {
		return;
	}
	strBuffer_[newLen] = 0;
	strLength_ = newLen;
}

void ZString::resetBuffer()
{
	// 若有使用堆空间，则释放堆空间
	if (isStoreInHeapMem_) {
		ZSAFE_FREE(strBuffer_);
	}
	isStoreInHeapMem_ = false;
	strLength_ = 0;
	bufferSize_ = sizeof(stackBuffer_);
	memset(stackBuffer_, 0, sizeof(stackBuffer_));
	strBuffer_ = &stackBuffer_[0];
}

void ZString::assignValue(const char* str, const int len)
{
	if (str == nullptr || len <= 0) {
		return;
	}
	ensureBuffer(len);
#ifdef _MSVC
	strncpy_s(strBuffer_, bufferSize_, str, len);
#else
	strncpy(strBuffer_, str, len);
#endif
	strBuffer_[len] = 0; // 强制添加字符串结束符
	strLength_ = len;
}

void ZString::append(const char* str, const int len /*= -1*/)
{
	if (str == nullptr) {
		return;
	}
	int strLen = len;
	if (strLen < 0) {
		strLen = (int)strlen(str);
	}
	if (strLen <= 0) {
		return;
	}

	ensureBuffer(strLen);
#ifdef _MSVC
	strncpy_s(strBuffer_ + strLength_, bufferSize_ - strLength_, str, strLen);
#else
	strncpy(strBuffer_ + strLength_, str, strLen);
#endif
	strLength_ += strLen;
	strBuffer_[strLength_] = 0;
}

void ZString::append(const char ch)
{
	char str[2] = { 0 };
	str[0] = ch;
	append(str, 1);
}

void ZString::format(const char* format, ...)
{
	std::vector<char> buffer;
#ifdef MSVC
	va_list argList;
	va_start(argList, format);
	int len = vsnprintf(0, 0, format, argList);
	buffer.resize(z_align8(len + 1));
	vsprintf_s(buffer.data(), buffer.size(), format, argList);
	va_end(argList);
#else
	// 求字符串长度
	va_list argList;
	va_start(argList, format);
	int len = vsnprintf(0, 0, format, argList);
	va_end(argList);
	buffer.resize(z_align8(len + 1));
	// 获取字符串内容
	va_list argList2;
	va_start(argList2, format);
	vsprintf(buffer.data(), format, argList2);
	va_end(argList2);
#endif
	resetBuffer();
	ensureBuffer((int)buffer.size());
	assignValue(buffer.data(), len);
}

void ZString::appendFormat(const char* format, ...)
{
	std::vector<char> buffer;
#ifdef MSVC
	va_list argList;
	va_start(argList, format);
	int len = vsnprintf(0, 0, format, argList);
	buffer.resize(z_align8(len + 1));
	vsprintf_s(buffer.data(), buffer.size(), format, argList);
	va_end(argList);
#else
	// 求字符串长度
	va_list argList;
	va_start(argList, format);
	int len = vsnprintf(0, 0, format, argList);
	va_end(argList);
	buffer.resize(z_align8(len + 1));
	// 获取字符串内容
	va_list argList2;
	va_start(argList2, format);
	vsprintf(buffer.data(), format, argList2);
	va_end(argList2);
#endif
	append(buffer.data());
}

size_t ZString::hashCode() const noexcept
{
	return ZHash::encodeStr32(getData(), getLength());
}

size_t ZString::hashCode(const ZString& str)
{
	return ZHash::encodeStr32(str.getData(), str.getLength());
}

bool ZString::isEmptyStr(const char* str)
{
	if (str == nullptr || str[0] == 0) {
		return true;
	}
	return false;
}

ZSpVecStdString ZString::split(const ZString& strIn, char ch, bool isTrim /*= true*/)
{
	auto sharedList = std::make_shared<ZVecStdString>();
	sharedList->clear();
	if (strIn.getLength() == 0) {
		return sharedList;
	}
	// 预留空间保存字符串结束符
	std::vector<char> buffer(strIn.getLength() + 2);
#ifdef _MSVC
	strncpy_s(&buffer[0], buffer.size(), strIn.getData(), strIn.getLength() + 1);
#else
	strncpy(&buffer[0], strIn.getData(), strIn.getLength() + 1);
#endif
	char* beginPos = &buffer[0];
	while (true) {
		char* endPos = strchr(beginPos, ch);
		ZString temp("");
		if (endPos == nullptr) {
			temp = beginPos;
			if (isTrim) {
				trim(temp, temp);
			}
			if (temp.getLength() > 0) {
				sharedList->push_back(std::string(temp.getData()));
			}
			break;
		}
		*endPos = 0; // 将分隔的字符，修改为字符串的结束符
		temp = beginPos;
		if (isTrim) {
			trim(temp, temp);
		}
		if (temp.getLength() > 0) {
			sharedList->push_back(std::string(temp.getData()));
		}
		beginPos = endPos + 1;
	}
	return sharedList;
}

void ZString::trimLeft(const ZString& strIn, ZString& strOut)
{
	int pos = 0;
	for (; pos < strIn.getLength(); pos++) {
		if (strIn[pos] != 32
			&& strIn[pos] != '\r'
			&& strIn[pos] != '\n'
			&& strIn[pos] != '\t') {
			break;
		}
	}
	ZString tmpStr(strIn);
	if (pos > 0 && pos < strIn.getLength()) {
		tmpStr.assign(strIn.getData() + pos, strIn.getLength() - pos);
	}
	strOut = tmpStr;
}

void ZString::trim()
{
	ZString::trim(*this, *this);
}

void ZString::trimLeft()
{
	ZString::trimLeft(*this, *this);
}

void ZString::trimRight()
{
	ZString::trimRight(*this, *this);
}

void ZString::trimRight(const ZString& strIn, ZString& strOut)
{
	int pos = strIn.getLength() - 1;
	for (; pos >= 0; pos--) {
		if (strIn[pos] != 32
			&& strIn[pos] != '\r'
			&& strIn[pos] != '\n'
			&& strIn[pos] != '\t') {
			break;
		}
	}
	ZString tmpStr(strIn);
	if (pos >= 0 && pos < strIn.getLength()) {
		int len = strIn.getLength() - (strIn.getLength() - pos - 1);
		tmpStr.assign(strIn.getData(), len);
	}
	strOut = tmpStr;
}

void ZString::trim(const ZString& strIn, ZString& strOut)
{
	int  leftPos = 0;
	for (; leftPos < strIn.getLength(); leftPos++) {
		if (strIn[leftPos] != 32
			&& strIn[leftPos] != '\r'
			&& strIn[leftPos] != '\n'
			&& strIn[leftPos] != '\t') {
			break;
		}
	}
	if (leftPos == strIn.getLength()) {
		return;
	}
	int  rightPos = strIn.getLength() - 1;
	for (; rightPos >= 0; rightPos--) {
		if (strIn[rightPos] != 32
			&& strIn[rightPos] != '\r'
			&& strIn[rightPos] != '\n'
			&& strIn[rightPos] != '\t') {
			break;
		}
	}
	ZString tmpStr("");
	tmpStr.assign(strIn.getData() + leftPos, rightPos - leftPos + 1);
	strOut = tmpStr;
}

int ZString::toInt() const
{
	if (!ZString::isValidInteger(*this)) {
		return 0;
	}
	return ZString::toInt(*this);
}

int64_t ZString::toInt64() const
{
	if (!ZString::isValidInteger(*this)) {
		return 0;
	}
	return ZString::toInt64(*this);
}

float ZString::toFloat() const
{
	if (!ZString::isValidFloat(*this)) {
		return 0;
	}
	return ZString::toFloat(*this);
}

double ZString::toDouble() const
{
	if (!ZString::isValidFloat(*this)) {
		return 0;
	}
	return ZString::toDouble(*this);
}

int ZString::toInt(const ZString& str)
{
	return atoi(str.getData());
}

int64_t ZString::toInt64(const ZString& str)
{
#ifdef _MSVC
	return _atoi64(str.getData());
#else
	return atoll(str.getData());
#endif
}

float ZString::toFloat(const ZString& str)
{
	return (float)atof(str.getData());
}

double ZString::toDouble(const ZString& str)
{
	return atof(str.getData());
}

bool ZString::isValidInteger(const ZString& str)
{
	bool isValid = true;
	for (int i = 0; i < str.getLength(); i++) {
		// 第1个位置是负号
		if (i == 0 && str[i] == '-') {
			continue;
		}
		if (str[i] < '0' || str[i] > '9') {
			isValid = false;
			break;
		}
	}
	return isValid;
}

bool ZString::isValidFloat(const ZString& str)
{
	bool isValid = true;
	for (int i = 0; i < str.getLength(); i++) {
		if ((str[i] < '0' || str[i] > '9')
			&& str[i] != '.'
			&& str[i] != '+' && str[i] != '-'
			&& str[i] != 'E' && str[i] != 'e')
		{
			isValid = false;
			break;
		}
	}
	return isValid;
}

void ZString::copyString(char* dstStr, const int dstSize, const ZString& srcStr)
{
	ZString::copyString(dstStr, dstSize, srcStr.getData(), srcStr.getLength());
}

void ZString::copyString(char* dstStr, const int dstSize, const char* srcStr, const int srcLen /*= -1*/)
{
	if (dstStr == nullptr || dstSize <= 0) {
		return;
	}
	if (srcStr == nullptr) {
		dstStr[0] = 0; // 将目标字符串缓存置空
		return;
	}
	int copyLen = srcLen;
	if (copyLen < 0) {
		copyLen = (int)strlen(srcStr);
	}
	copyLen = min(dstSize - 1, copyLen);
	if (copyLen > 0) {
#ifdef _MSVC
		strncpy_s(dstStr, dstSize, srcStr, copyLen);
#else
		strncpy(dstStr, srcStr, copyLen);
#endif
	}
	dstStr[copyLen] = 0;
}

// 判断主串中是否包含字串
bool ZString::normalContain(const char* mainStr, const int mainLen, const char* subStr, const int subLen, bool ingoreCase /*= false*/)
{
	if (subStr == nullptr || subLen <= 0) {
		return true;
	}
	if (mainStr == nullptr || mainLen <= 0) {
		return false;
	}
	bool flag = false;
	for (int i = 0; i <= mainLen - subLen; i++) {
		int ret = 0;
		if (ingoreCase) {
#ifdef _MSVC
			ret = _strnicmp(mainStr + i, subStr, subLen);
#else
			ret = strncasecmp(mainStr + i, subStr, subLen);
#endif
		}
		else {
			ret = strncmp(mainStr + i, subStr, subLen);
		}
		if (ret == 0) {
			flag = true;
			break;
		}
	}
	return flag;
}

int ZString::compare(const char* str1, const char* str2, const int lenCompare, bool ingoreCase /*= false*/)
{
	int ret = 0;
	if (ingoreCase) {
#ifdef _MSVC
		ret = _strnicmp(str1, str2, (size_t)lenCompare);
#else
		ret = strncasecmp(str1, str2, (size_t)lenCompare);
#endif
	}
	else {
		ret = strncmp(str1, str2, (size_t)lenCompare);
	}
	return ret;
}

int ZString::compare(const std::string& str1, const std::string& str2, bool ingoreCase /*= false*/)
{
	int lenCompare = min(str1.length(), str2.length()) + 1;
	return compare(str1.c_str(), str2.c_str(), lenCompare);
}

int ZString::compare(const ZString& str1, const ZString& str2, bool ingoreCase /*= false*/)
{
	int lenCompare = min(str1.getLength(), str2.getLength()) + 1;
	return compare(str1.getData(), str2.getData(), lenCompare);
}
ZUTILS_NS_END
