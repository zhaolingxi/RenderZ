#include "renderz_main_page.h"
#include<QApplication>
#include<QDir>
#include<QDirIterator>
RenderZMainPage::RenderZMainPage(QWidget* parent, const QString& viewName)
{
}

RenderZMainPage::~RenderZMainPage()
{
}

bool RenderZMainPage::createPage()
{
	bool ret = true;
	initUI();
	show();
	return ret;
}

void RenderZMainPage::initUI()
{
	mainLayout_ = new QGridLayout(this);
	mainSideBarLeft_ = new QWidget(this);
	mainSideBarRight_ = new QWidget(this);
	pageViewLayout_ = new QVBoxLayout();
	pageViewLayoutTop_ = new QHBoxLayout();
	pageViewLayoutMid_ = new QHBoxLayout();
	pageViewLayoutBtm_ = new QHBoxLayout();

	addPushBtm(pageViewLayoutTop_);
	addPushBtm(pageViewLayoutMid_);
	addPushBtm(pageViewLayoutBtm_);

	pageViewLayout_->addLayout(pageViewLayoutTop_);
	pageViewLayout_->addLayout(pageViewLayoutMid_);
	pageViewLayout_->addLayout(pageViewLayoutBtm_);

	mainLayout_->addLayout(pageViewLayout_,0,0);
}

void RenderZMainPage::addPushBtm(QHBoxLayout*& ioLayout)
{

	auto addbtm = [ioLayout]() {
		QPushButton* pb1 = new QPushButton();
		ioLayout->addWidget(pb1);
		};

	for (int i = 0; i < 2;i++) {
		addbtm();
	}

}

void RenderZMainPage::tempLoadTheme(QApplication* app)
{
	std::string appSourceDir = _APP_SOURCE_DIR;
	std::string pagesThemeDir = appSourceDir + "/../pages/resources/theme/default/renderz_app";
	std::shared_ptr<QString> qstrptr=readQssFiles(pagesThemeDir.c_str());
	if (qstrptr) {
		if (!app) {
			((QApplication*)QApplication::instance())->setStyleSheet((*qstrptr));
		}
		else {
			app->setStyleSheet((*qstrptr));
		}
	}

}

std::shared_ptr<QString> RenderZMainPage::readQssFiles(const QString& dirPath)
{
	auto allQssStr = std::make_shared<QString>("");
	QDir dir(dirPath);
	if (!dir.exists()) {
		return allQssStr;
	}

	QStringList qssFileFilters;
	qssFileFilters << QString("*.qss");
	// 遍历文件夹中的qss文件
	QDirIterator dirIterator(dirPath, qssFileFilters,
		QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
	QStringList fileDirList;
	while (dirIterator.hasNext()) {
		dirIterator.next();
		QFileInfo fileInfo = dirIterator.fileInfo();
		// 忽略路径和文件名称中包含'~'的样式
		QString qssFilePath = fileInfo.filePath();
		if (qssFilePath.indexOf('~') >= 0) {
			continue;
		}
		QString fileDir = fileInfo.absoluteFilePath();
		fileDirList.append(fileDir);
	}

	// 文件夹中不包含qss文件
	if (fileDirList.isEmpty()) {
		return allQssStr;
	}

	for (int i = 0; i < fileDirList.size(); i++)
	{
		QString strPath = fileDirList.at(i);
		QFile qssfile(strPath);
		if (qssfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QByteArray fileData = qssfile.readAll();
			QString strStyle = fileData.data();
			*allQssStr += strStyle;
			qssfile.close();
		}
	}
	return allQssStr;
}



void RenderZMainPage::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}