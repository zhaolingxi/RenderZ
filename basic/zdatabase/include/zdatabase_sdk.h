#pragma once

// ���ֿռ䶨��
#define ZDataBaseNS zdatabase
#define ZDATABASE_NS_BEGIN  namespace ZDataBaseNS {
#define ZDATABASE_NS_END  }

#ifdef _ZDATABASE_EXPORTS
#if defined(WIN32) || defined(WIN64)
// Windowsƽ̨�����ӿ�
#define ZDATABASE_API __declspec(dllexport)
#else
// Linuxƽ̨�����ӿ�
#define ZDATABASE_API 
#endif
#else
#if defined(WIN32) || defined(WIN64)
// Windowsƽ̨����ӿ�
#define ZDATABASE_API __declspec(dllimport)
#else
// Linuxƽ̨����ӿ�
#define ZDATABASE_API 
#endif
#endif
#include "zutils.h"
#include "zcore.h"

