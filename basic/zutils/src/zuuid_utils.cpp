#include "zuuid_utils.h"
ZUTILS_NS_BEGIN
void ZUUIDUtils::generateUUIDString(const char* str,ZString& uuid)
{
	
}

void ZUUIDUtils::generateUUIDINT(const char* str, int64_t& uuid)
{
	auto ans = ZHash::encodeStr(str);
	uuid = ans;
}
ZUTILS_NS_END