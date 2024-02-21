#pragma once
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4819)
#define NOMINMAX
#endif
#include "zcore_sdk.h"
#include "log4z.h"
#include "zcore_singleton.h" // the singleton manager
#include "task_scheduler.h"
using namespace ZCoreNS;
