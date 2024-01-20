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
//#include "renderz_main_view.h"

int main(int argc, char* argv[])
{
	int ret;
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	std::shared_ptr<QApplication> app = std::make_shared<QApplication>(argc, argv);
	QString appDir = QApplication::applicationDirPath();
	QDir::setCurrent(appDir);
	qDebug() << "appDir: " << appDir;

	//ThresholdsetPopUpWidget* ptest = new ThresholdsetPopUpWidget(nullptr,"");
	//ptest->createView();
	//ptest->show();

	ret = app->exec();
	return ret;
}
