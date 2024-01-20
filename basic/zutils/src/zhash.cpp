#include <cstring>
#include "encode/zhash.h"
#include "xxhash/xxhash.h"
/**********************************************************************
 *@brief  HASH函数
 ***********************************************************************/
ZUTILS_NS_BEGIN
uint32_t ZHash::encodeStr(const char *str, const int len /*= -1*/)
{
	int strLen = len;
	if (strLen < 0) {
		strLen = (str == nullptr) ? 0 : (int)strlen(str);
	}
	return XXH32((void*)str, strLen, HASH_SEED);
}

uint32_t ZHash::encode(const uint8_t *dataBuffer, const int dataSize)
{
	int dataLen = dataSize;
	if (dataLen < 0 ) {
		dataLen = 0;
	}
	return XXH32((void*)dataBuffer, dataLen, HASH_SEED);
}
ZUTILS_NS_END