#include "plugin_main.h"
#include "renderz_main_page.h"
#include"renderz_main_page_define.h"
#include"zuuid_utils.h"

RENDERZ_MAIN_PAGE_API void GetPluginViewInfo(ZLibPluginInfo* pluginViewInfo)
{
	ZString uuid;
	ZUUIDUtils::generateUUIDString(PAGEID, uuid);
	if(!uuid.isEmpty())pluginViewInfo->setUUID(uuid);
}

RENDERZ_MAIN_PAGE_API bool InstallPluginView()
{
	RenderZMainPage* page= new RenderZMainPage(nullptr);
	if (page) {
		page->createPage();
	}
	return false;
}
