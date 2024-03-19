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
#include "log4z.h"
#include"zlib_loader.h"
#include"zlib_plugin_info.h"


int main(int argc, char* argv[])
{
	int ret=0;
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	std::shared_ptr<QApplication> app = std::make_shared<QApplication>(argc, argv);

	LoggerId logId = ILog4zManager::getInstance()->createLogger("mainlogger");


	QString appDir = QApplication::applicationDirPath();
	QDir::setCurrent(appDir);
	qDebug() << "appDir: " << appDir;

	ILog4zManager::getInstance()->setLoggerPath(logId,appDir.toStdString().c_str());
	ILog4zManager::getInstance()->setLoggerName(logId,"mainlogger");
	ILog4zManager::getInstance()->setLoggerOutFile(logId, 1);

	ILog4zManager::getInstance()->start();
	LOGFMTI("ILog4zManager start");

	std::vector<std::string> pluginPages{ "renderz_main_page", };
	for (auto& pluginPage : pluginPages) {
		std::string strdll = appDir.toUtf8().data();
		strdll=strdll + "/" + pluginPage.c_str() + ".dll";
		auto ret = ZLibLoader::loadLib(strdll.c_str());
		typedef void(__cdecl* ModuleInfo)(ZLibPluginInfo*);

		auto func = (ModuleInfo)ZLibLoader::getProcAddress(ret,"GetPluginViewInfo");
		auto pluginModuleInfo = std::make_shared<ZLibPluginInfo*>();
		func(*pluginModuleInfo);


		auto func2 = ZLibLoader::getProcAddress(ret, "InstallPluginView");
		func2();
	}

	//RenderZMainPage* pMianPage = new RenderZMainPage(nullptr,"RenderZMainPage");
	//pMianPage->tempLoadTheme(app.get());
	//pMianPage->createPage();

//	testcase::threadTest01();
//	testcase::timeTest01();
//	testcase::taskSechTest01();
//	testcase::taskSqliteTest02();

	ret = app->exec();

	ILog4zManager::getInstance()->stop();
	return ret;
}
