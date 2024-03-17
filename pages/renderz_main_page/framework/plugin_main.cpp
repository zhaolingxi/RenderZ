#include "plugin_main.h"
#include "renderz_main_page.h"
#include"renderz_main_page_define.h"
#include"zuuid_utils.h"

RENDERZ_MAIN_PAGE_API void GetPluginViewInfo(zutils::ZLibPluginInfo* pluginViewInfo)
{
	zutils::ZString uuid;
	zutils::ZUUIDUtils::generateUUIDString(PAGEID, uuid);
	if(!uuid.isEmpty())pluginViewInfo->setUUID(uuid);
}

//RENDERZ_MAIN_PAGE_API bool InstallPluginView(const QString& viewName,
//	UIViewManager* viewManager, UIModelManager* modelManager)
//{
//	if (viewName == kUIThresholdset2ViewName) {
//		return install_plugin_view<Thresholdset2View>(viewName, viewManager, modelManager,kUIThresholdset2ViewID);
//	}
//	return false;
//}
