#include "renderz_main_page.h"
#include<QApplication>
#include<QDir>
#include<QDirIterator>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QStandardItem>

RenderZMainPage::RenderZMainPage(QWidget* parent, const QString& viewName)
{
	setObjectName(viewName);
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

bool RenderZMainPage::createLayoutTop(QHBoxLayout*& pageViewLayoutTop)
{
	std::string appSourceDir = _APP_SOURCE_DIR;
	std::string pagesThemeDir = appSourceDir + "/../assert/image/icon/login.bmp";
	QIcon titleIcon(pagesThemeDir.c_str());
	QPushButton* titleIconbtn = new QPushButton();
	titleIconbtn->setIcon(titleIcon);
	QLabel *titleNameLab=new QLabel(tr("RenderZ_v0.1"), this);
	QLineEdit* serachLine = new QLineEdit(this);
	QPushButton* personalPage = new QPushButton(this);
	QPushButton* globalSetting=new QPushButton(this);
	QPushButton* minBtn = new QPushButton(this);
	minBtn->setObjectName("minBtn");
	QPushButton* maxBtn = new QPushButton(this);
	minBtn->setObjectName("maxBtn");
	QPushButton* closeBtn=new QPushButton(this);
	minBtn->setObjectName("closeBtn");


	pageViewLayoutTop->addWidget(titleIconbtn);
	pageViewLayoutTop->addWidget(titleNameLab);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(serachLine);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(personalPage);
	pageViewLayoutTop->addWidget(globalSetting);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(minBtn);
	pageViewLayoutTop->addWidget(maxBtn);
	pageViewLayoutTop->addWidget(closeBtn);
	return true;
}

bool RenderZMainPage::createLayoutMid(QHBoxLayout*& pageViewLayoutMid)
{
	mainSideBarLeft_ = new QWidget(this);
	leftNavigModel_ = new ZQtNavigatorModel();
	// 添加一些示例节点
	TreeItem* rootNode = leftNavigModel_->getRootItem();
	TreeItem* item1 = new TreeItem("Item 1", rootNode);
	TreeItem* item2 = new TreeItem("Item 2", rootNode);
	TreeItem* childItem = new TreeItem("Child Item", item1);
	// 构建树形结构
	item1->appendChild(childItem);
	item2->appendChild(new TreeItem("Child Item 2", item2));
	rootNode->appendChild(item1);
	rootNode->appendChild(item2);

	

	leftNavigView_ = new ZQtNavigatorView(nullptr, leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);
	std::string modelPath = MODEL_PATH; modelPath += "AfricanHead/african_head.obj";
	std::cout << modelPath << std::endl;
	mianViewer_->load3DModel(modelPath);

	mainSideBarRight_ = new SideSettingView(this);

	pageViewLayoutMid->addWidget(leftNavigView_);
	pageViewLayoutMid->addWidget(mianViewer_);
	pageViewLayoutMid->addWidget(mainSideBarRight_);
	return true;
}

bool RenderZMainPage::createLayoutBtm(QHBoxLayout*& pageViewLayoutBtm)
{
	QLabel* messageLab = new QLabel(tr("tips:"));
	QPushButton* axisBtn = new QPushButton();
	pageViewLayoutBtm_->addWidget(messageLab);
	pageViewLayoutBtm_->addStretch();
	pageViewLayoutBtm_->addWidget(axisBtn);
	return true;

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

	createLayoutTop(pageViewLayoutTop_);
	createLayoutMid(pageViewLayoutMid_);
	createLayoutBtm(pageViewLayoutBtm_);

	pageViewLayout_->addLayout(pageViewLayoutTop_);
	pageViewLayout_->addLayout(pageViewLayoutMid_);
	pageViewLayout_->addLayout(pageViewLayoutBtm_);

	mainLayout_->addLayout(pageViewLayout_,0,0);
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

