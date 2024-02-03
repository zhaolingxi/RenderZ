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



int main(int argc, char* argv[])
{
	int ret=0;
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	std::shared_ptr<QApplication> app = std::make_shared<QApplication>(argc, argv);
	QString appDir = QApplication::applicationDirPath();
	QDir::setCurrent(appDir);
	qDebug() << "appDir: " << appDir;

	RenderZMainPage* pMianPage = new RenderZMainPage(nullptr,"RenderZMainPage");
	pMianPage->tempLoadTheme(app.get());
	pMianPage->createPage();

	ret = app->exec();
	return ret;
}
