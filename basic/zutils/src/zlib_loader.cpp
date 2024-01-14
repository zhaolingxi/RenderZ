#include "zlib_loader.h"
//#include "zmisc_utils.hpp"
//#include "zfile_utils.h"
ZUTILS_NS_BEGIN


ZLibLoader::ZLibLoader(const char* libFilePath)
{
	if (libFilePath) {
		libFilePath_ = libFilePath;
	}
}

ZLibLoader::~ZLibLoader()
{

}

bool ZLibLoader::load()
{
	return false;
}

bool ZLibLoader::isLoaded()
{
	return false;
}

ZLibLoader::ProcAddress ZLibLoader::resolve(const char* procName)
{
	return ProcAddress();
}

void ZLibLoader::unload()
{
}

ZLibLoader::ModHandler ZLibLoader::loadLib(const char* libFilePath)
{

}

ZLibLoader::ProcAddress ZLibLoader::getProcAddress(ModHandler modHandler, const char* procName)
{
}


void ZLibLoader::freeLib(ModHandler modHandler) 
{
}

ZUTILS_NS_END


