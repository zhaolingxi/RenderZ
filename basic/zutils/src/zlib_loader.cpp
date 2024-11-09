#include "zlib_loader.h"
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
	modHandler_ = ZLibLoader::loadLib(libFilePath_.c_str());
	if (modHandler_ == nullptr) {
		return false;
	}
	return true;
}

bool ZLibLoader::isLoaded()
{
	return (modHandler_ != nullptr);
}

ZLibLoader::ProcAddress ZLibLoader::resolve(const char* procName)
{
	return ZLibLoader::getProcAddress(modHandler_, procName);
}

void ZLibLoader::unload()
{
	if (modHandler_ != nullptr) {
		ZLibLoader::freeLib(modHandler_);
		modHandler_ = nullptr;
	}
}

ZLibLoader::ModHandler ZLibLoader::loadLib(const char* libFilePath)
{
//文件实体判空（待补充）
	ModHandler hDll = nullptr;
#ifdef _MSVC
	hDll = ::LoadLibraryA(libFilePath);
	if (hDll == nullptr) {
		DWORD err = GetLastError();
	}
#else
	hDll = dlopen(libFilePath, RTLD_NOW | RTLD_GLOBAL);
	if (hDll == nullptr) {
	}
#endif
	return hDll;
}

ZLibLoader::ProcAddress ZLibLoader::getProcAddress(ModHandler modHandler, const char* procName)
{
	if (modHandler == nullptr || procName == nullptr || procName[0] == 0) {
		return nullptr;
	}
	ProcAddress fnProc = nullptr;
#ifdef _MSVC
	fnProc = ::GetProcAddress(modHandler, procName);
	if (fnProc == nullptr) {
		DWORD err = GetLastError();
	}
#else
	fnProc = dlsym(modHandler, procName);
	if (fnProc == nullptr) {
	}
#endif
	return fnProc;
}


void ZLibLoader::freeLib(ModHandler modHandler) 
{
#ifdef _MSVC
	::FreeLibrary(modHandler);
#else
	dlclose(modHandler);
#endif
}

ZUTILS_NS_END


