#include"zlib_plugin_info.h"
ZUTILS_NS_BEGIN

void ZLibPluginInfo::setUUID(const ZString uuid)
{
	libuuid_ = uuid;
}

void ZLibPluginInfo::getUUID(ZString& uuid) 
{
	uuid = libuuid_;
};
ZUTILS_NS_END