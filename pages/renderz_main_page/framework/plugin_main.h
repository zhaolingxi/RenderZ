#pragma once
#include "renderz_main_page_sdk.h"
#include"zlib_plugin_info.h"
#ifdef __cplusplus
extern "C"
{
#endif
	RENDERZ_MAIN_PAGE_API void GetPluginViewInfo(zutils::ZLibPluginInfo* pluginViewInfo);

	//RENDERZ_MAIN_PAGE_API bool InstallPluginView(const QString& viewName);

	//RENDERZ_MAIN_PAGE_API bool InstallPluginView(const QString& viewName,
	//	UIViewManager* viewManager, UIModelManager* modelManager);

#ifdef __cplusplus
};
#endif
