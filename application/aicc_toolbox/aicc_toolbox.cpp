#include <QApplication>
#include <QLibraryInfo>
#include <QProcess>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <memory>
#include <any>
#include "aautils.h"
#include "aacore.h"
#include "aanetwork.h"
#include "aagui.h"
#include "toolbox_main_view.h"
#include <QTimer>
int main(int argc, char* argv[])
{
	qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(common_resources);
	QString appDir = QApplication::applicationDirPath();
	QDir::setCurrent(appDir);
	qDebug() << "appDir: " << appDir;
	QString path = QLibraryInfo::location(QLibraryInfo::DataPath);
	qDebug() << "QLibraryInfo path = " << path;

	//////////////////////////////////////////////////////
	// 1、初始化基础框架
#ifdef _DEBUG
	std::string appSourceDir = _APP_SOURCE_DIR;
#endif
#ifdef _DEBUG
	std::string configDir = appSourceDir + "/config/aicc_toolbox";
#else
	std::string configDir = "./config/aicc_toolbox";
#endif
	std::string logCfg = *AAPathUtils::joinPath(configDir, "aicc_toolbox_log.ini");
	AACoreSingleton::initCoreFramework(configDir.c_str(), logCfg.c_str());
	// 启动主线程轮询定时器
	QTimer timer;
	QObject::connect(&timer, &QTimer::timeout, []() { AACoreSingleton::singleLoop(); });
	int intervalMs = 10;
	auto coreConfig = AACoreSingleton::getConfigManager()->getCoreConfig();
	if (coreConfig != nullptr) {
		intervalMs = coreConfig->getMainSingleLoopIntervalMS();
	}
	timer.setInterval(intervalMs);
	LOGFMTI("---main---SingleLoopIntervalMS = %d", intervalMs);
	timer.start();

	//////////////////////////////////////////////////////
	// 2、初始化网络模块
	AANetWorkSingleton::initServerFramework();
	// 启动Http的事件轮询线程
	AANetWorkSingleton::getHttpFramework()->startEvtLoopThread();

	//////////////////////////////////////////////////////
	// 3、初始化界面框架
	AAGUISingleton::initGUIFramework();
#ifdef _DEBUG
	std::string pagesThemeDir = appSourceDir + "/../pages/resources/theme";
	std::string pagesLangDir = appSourceDir + "/../pages/resources/language";
	AAGUISingleton::loadTheme(pagesThemeDir.c_str(), "default", true);
#else
	std::string pagesThemeDir = "./resources/theme";
	std::string pagesLangDir = "./resources/language";
	AAGUISingleton::loadTheme(pagesThemeDir.c_str(), "default", false);
#endif
	// 界面界面语言 zh 简体中文
	AAGUISingleton::loadLanguage(pagesLangDir.c_str(), "zh");
	AAGUISingleton::changeTheme("default");
	AAGUISingleton::changeLanguage("zh");
	//////////////////////////////////////////////////////
	// 4、加载业务模块插件
	std::vector<std::string> logicPluginNames = { "sdk_adapter",
												 "dbcp_control",
												 "topo_storage",
												 "dcu_connector",
												 "dcu_storage",
												 "log_center",
												 "bag_center",
												 "dcu_monitor",
												 "topic_monitor",
												 "threshold_storage" };
	for (auto& pluginName : logicPluginNames) {
		bool ret = AACoreSingleton::loadPlugin(appDir.toUtf8().data(), pluginName.c_str());
		LOGFMTI("-----main()--logics--loadPlugin: %s finish ret = %d", pluginName.c_str(), ret);
		if (!ret) {
			LOGFMTE("-----main()--logics--loadPlugin: %s failed!!!", pluginName.c_str());
		}
	}
	bool ret = AACoreSingleton::installAllModules();
	if (!ret) {
		LOGFMTE("----main()--logics-----install all modules failed!!!");
		return -1;
	}

	//////////////////////////////////////////////////////
	// 4、加载页面插件
	ret = true;
	std::vector<std::string> pluginPages{ "dcumanage_page",
										 "bagrecord_page",
										 "bagreplay_page",
										 "logmonitor_page",
										 "dcumonitor_page",
										 "msgmonitor_page",
										 "thresholdset2_page",
										 "topoview_page", };
	for (auto& pluginPage : pluginPages) {
		ret = AAGUISingleton::loadPlugin(appDir.toUtf8().data(), pluginPage.c_str());
		LOGFMTI("----main()--pages--loadPlugin: %s finish ret = %d", pluginPage.c_str(), ret);
		if (!ret) {
			LOGFMTE("----main()--pages--loadPlugin: %s failed!!!", pluginPage.c_str());
		}
	}

	//////////////////////////////////////////////////////
	// 5、加载临时主页和需要显示的子页面
	ret = preinstall_plugin_view<ToolBoxMainView>("ToolBoxMainView",
		AAGUISingleton::getViewManager(),
		AAGUISingleton::getModelManager());
	if (!ret) {
		LOGFMTE("----main-------preinstall ToolBoxMainView failed!!!");
		return -1;
	}
	auto toolboxMainView = AAGUISingleton::getViewManager()->getPreViewByName("ToolBoxMainView");
	if (toolboxMainView == nullptr) {
		LOGFMTE("----main---getPreViewByName--ToolBoxMainView-----toolboxMainView == nullptr");
		return -1;
	}
	AAGUISingleton::setMainView(toolboxMainView);
	std::vector<std::string> contentViewNames = { "DcuManageView",
												 "DcuConnectorView",
												 "BagRecordView",
												 "BagReplayView",
												 "LogMonitorView",
												 "DcuMonitorView",
												 "MsgMonitorView",
												 "Thresholdset2View",
												 "TopoViewView" };
	ret = AAGUISingleton::installViewChains(contentViewNames);
	if (!ret) {
		LOGFMTE("----main()--pages--install contentViewNames failed!!!");
		return -1;
	}

	ret = toolboxMainView->createView();
	if (!ret) {
		LOGFMTE("----main-------install toolboxMainView createView failed!!!");
		return -1;
	}
	toolboxMainView->show();
	toolboxMainView->adjustSize();
	app.exec();
	// 停止GUI插件模块
	AAGUISingleton::stopGUIFramework();
	// 停止服务插件模块
	AANetWorkSingleton::stopServerFramework();
	// 停止模块中的线程调度器，清空调度器中任务队列中的任务
	AACoreSingleton::stopCoreFramework();
	// 销毁基础框架中的管理器
	AANetWorkSingleton::exitServerFramework();
	// 当前退出界面插件时,存在内存重复释放的问题，需要继续排查具体原因
	//AAGUISingleton::exitGUIFramework();
	AACoreSingleton::exitCoreFramework();
	return 0;
}
