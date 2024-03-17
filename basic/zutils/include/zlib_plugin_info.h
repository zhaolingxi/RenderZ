#pragma once
#include "zutils_sdk.h"
#include "zstring.h"
#include <unordered_map>
#include <vector>
ZUTILS_NS_BEGIN
class ZUTILS_API ZLibPluginInfo
{
public:
	ZLibPluginInfo() {
	};
	virtual ~ZLibPluginInfo() {};

	void setUUID(const zutils::ZString uuid);

	void getUUID(zutils::ZString& uuid) ;
private:
	zutils::ZString libuuid_;
	std::unordered_map < int64_t, std::shared_ptr < std::vector<ZLibPluginInfo> >> moduleChainMap_;
};
ZUTILS_NS_END