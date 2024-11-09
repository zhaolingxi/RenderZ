#include "zuuid_utils.h"
ZUTILS_NS_BEGIN
void ZUUIDUtils::generateUUIDINT(UUIDINFO& info, int64_t& uuid)
{
	ZString type = mod_str_.at(info.mod_type);
	ZString total = type + info.main_content + info.extend;
	generateUUIDINT(total.getData(), uuid);
}

void ZUUIDUtils::generateUUIDINT(const char* str, int64_t& uuid)
{
	auto ans = ZHash::encodeStr32(str);
	uuid = ans;
}
ZUTILS_NS_END