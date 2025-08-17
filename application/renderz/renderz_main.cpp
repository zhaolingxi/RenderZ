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
#include <QSurfaceFormat> // 【新增】包含这个头文件

int main(int argc, char* argv[])
{
	// 【核心修复】在创建任何窗口之前，设置全局默认的OpenGL格式
	QSurfaceFormat format;
	// 请求一个兼容模式的上下文，以支持旧的OpenGL函数
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	// 你可以指定一个版本，例如3.3，这对于ImGui来说足够了
	format.setVersion(3, 3);
	QSurfaceFormat::setDefaultFormat(format);
	// -----------------------------------------------------------------

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
