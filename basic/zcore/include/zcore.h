#pragma once
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4819)
#define NOMINMAX
#endif
#include "zcore_sdk.h"
#include "log4z.h"
#include "zcore_singleton.h" // the singleton manager
#include "thread_manager.h"
#include "thread_base.h"
#include "executer_manager.h"
#include "scheduler_manager.h"
#include "task_scheduler.h"
#include "thread_taskscheduler.h"
#include "nonthread_taskscheduler.h"
#include "async_peer.h"
#include "task_executer.h"

// config
//#include "config/config_manager.h"
//#include "config/config_base.h"
//#include "config/config_decorator.h"
//#include "config/json_config_impl.h" // json config
//#include "config/ini_file_config_impl.h" // ini config
// config decorator
//#include "config/core_config.h"
//#include "config/database_config.h"
//#include "config/gui_config.h"
//#include "config/network_config.h"
// logic module
//#include "logic/logic_module_base.h"
//#include "logic/logic_module_chain.h"
//#include "logic/logic_module_evtdispatcher.h"
//#include "logic/logic_module_manager.h"
//#include "logic/logic_module_notify_router.h"
//#include "logic/logic_plugin_entity.h"
//#include "logic/logic_plugin_manager.h"
//#include "logic/logic_plugin_module_info.h"
//#include "logic/logic_plugin_utils.hpp"
using namespace ZCoreNS;
