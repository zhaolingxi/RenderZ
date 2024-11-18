#include <QApplication>
#include <QLibraryInfo>
#include <QProcess>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QWidget>
#include <memory>
#include "zutils.h"
#include "zcore.h"
#include "renderz_main_page.h"
#include "test_case.h"

#include "zutils_sdk.h"
#include"zthread.h"
#include"zstring.h"
#include"test_case.h"
#include"zlib_loader.h"
#include"zlib_plugin_info.h"
#include"eventpp/callbacklist.h"

int main(int argc, char* argv[])
{
	int ret=0;
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	std::shared_ptr<QApplication> app = std::make_shared<QApplication>(argc, argv);

	QString appDir = QApplication::applicationDirPath();
	QDir::setCurrent(appDir);
	qDebug() << "appDir: " << appDir;

	std::vector<std::string> pluginPages{ "renderz_main_page", };
	for (auto& pluginPage : pluginPages) {
		std::string strdll = appDir.toUtf8().data();
		strdll=strdll + "/" + pluginPage.c_str() + ".dll";
		auto ret = ZLibLoader::loadLib(strdll.c_str());
		typedef void(* ModuleInfo)(ZLibPluginInfo*);

		auto func_GetPluginViewInfo = (ModuleInfo)ZLibLoader::getProcAddress(ret,"GetPluginViewInfo");
		auto pluginModuleInfo = std::make_shared<ZLibPluginInfo*>();
		func_GetPluginViewInfo(*pluginModuleInfo);

		typedef void(*InstallPluginViewFunc)();
		InstallPluginViewFunc func_InstallPluginView =reinterpret_cast<InstallPluginViewFunc>(ZLibLoader::getProcAddress(ret, "InstallPluginView"));
		func_InstallPluginView();
	}


	ret = app->exec();

	return ret;
}
