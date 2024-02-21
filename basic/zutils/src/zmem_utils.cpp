#include "zmem_utils.h"
#include <cstring>
#ifdef _MSVC
#include <memory.h>
#include <windows.h>
#endif
#include <stdexcept>
ZUTILS_NS_BEGIN
void ZMemUtils::memCopy(void *dst, const int dstSize, const void *src, const int srcSize)
{
	if (dstSize < srcSize) {
		throw std::runtime_error("dst memory not enough");
	}
#ifdef _MSVC
	memcpy_s(dst, dstSize, src, srcSize);
#else
	memcpy(dst, src, srcSize);
#endif
}
ZUTILS_NS_END