#pragma once

// ���ֿռ䶨��
#define ZMathNS zmath
#define ZMATH_NS_BEGIN namespace ZMathNS {
#define ZMATH_NS_END }

#define HASGSL 1

#ifdef _ZMATH_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windowsƽ̨�����ӿ�
#define ZMATH_API __declspec(dllexport)
#else
// Linuxƽ̨�����ӿ�
#define ZMATH_API
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windowsƽ̨����ӿ�
#define ZMATH_API __declspec(dllimport)
#else
// Linuxƽ̨����ӿ�
#define ZMATH_API 
#endif
#endif
