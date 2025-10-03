#include "renderz_main_page.h"
#include<QApplication>
#include<QDir>
#include<QDirIterator>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QStandardItem>
#include <QMessageBox> 
#include<plog/Log.h>
#include<filesystem>
namespace fs = std::filesystem;
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
	connectSlots();
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
	QPushButton* fileBtn = new QPushButton(this);
	QPushButton* saveBtn=new QPushButton(this);
	QPushButton* settingBtn = new QPushButton(this);
	settingBtn->setObjectName("settingBtn");
	QPushButton* extBtn = new QPushButton(this);
	extBtn->setObjectName("maxBtn");
	QPushButton* aboutBtn=new QPushButton(this);
	aboutBtn->setObjectName("aboutBtn");


	pageViewLayoutTop->addWidget(titleIconbtn);
	pageViewLayoutTop->addWidget(titleNameLab);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(fileBtn);
	pageViewLayoutTop->addWidget(saveBtn);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(serachLine);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(settingBtn);
	pageViewLayoutTop->addWidget(extBtn);
	pageViewLayoutTop->addWidget(aboutBtn);
	return true;
}

bool RenderZMainPage::createLayoutMid(QHBoxLayout*& pageViewLayoutMid)
{

	// 1. ����Ҫɨ��ĸ�Ŀ¼
	std::string modelPathStr = MODEL_PATH;
	QString modelPath = QString::fromStdString(modelPathStr);

	// 2. ��ȡģ�͵ĸ�����
	QModelIndex rootIndex = QModelIndex();

	// 3. ���õݹ麯����ʼ���ģ��
	populateModelFromPath(leftNavigModel_, modelPath, rootIndex);

	leftNavigView_->setModel(leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);

	mainSideBarRight_ = new SideSettingView(this);

	pageViewLayoutMid->addWidget(leftNavigView_, 0);
	pageViewLayoutMid->addWidget(mianViewer_, 1);
	pageViewLayoutMid->addWidget(mainSideBarRight_, 0);
	return true;
}

QSplitter* RenderZMainPage::createMidSplitter()
{
	// �������� (�ⲿ�ִ��벻��)
	leftNavigModel_ = new ZQtNavigatorModel();
	leftNavigView_ = new ZQtNavigatorView(this); // ����ָ��������
	// ... (ģ������) ...
	leftNavigView_->setModel(leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);

	mainSideBarRight_ = new SideSettingView(this);

	// ����һ��ˮƽ����� QSplitter
	QSplitter* midSplitter = new QSplitter(Qt::Horizontal, this);

	// ��������ӵ� Splitter ��
	midSplitter->addWidget(leftNavigView_);
	midSplitter->addWidget(mianViewer_);
	midSplitter->addWidget(mainSideBarRight_);

	// (��ѡ) ���ó�ʼ����Դ�С����
	// �����������С��ҵı�����ԼΪ 1:4:1
	midSplitter->setSizes(QList<int>() << 150 << 600 << 150);

	// (��Ҫ) �����м����ͼΪ�Ǹ�������Ĳ���
	// �����ڴ�С�ı�ʱ��ֻ���м�Ĳ����ᰴ��������
	midSplitter->setStretchFactor(1, 1); // �ڶ������� (mianViewer_) ������������Ϊ1
	midSplitter->setStretchFactor(0, 0); // ��һ������������������Ϊ0
	midSplitter->setStretchFactor(2, 0); // ����������������������Ϊ0

	return midSplitter;
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
	mainSideBarLeft_ = new QWidget(this);
	leftNavigModel_ = new ZQtNavigatorModel();
	leftNavigView_ = new ZQtNavigatorView(nullptr);

	mainLayout_ = new QGridLayout(this);
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
	// �����ļ����е�qss�ļ�
	QDirIterator dirIterator(dirPath, qssFileFilters,
		QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
	QStringList fileDirList;
	while (dirIterator.hasNext()) {
		dirIterator.next();
		QFileInfo fileInfo = dirIterator.fileInfo();
		// ����·�����ļ������а���'~'����ʽ
		QString qssFilePath = fileInfo.filePath();
		if (qssFilePath.indexOf('~') >= 0) {
			continue;
		}
		QString fileDir = fileInfo.absoluteFilePath();
		fileDirList.append(fileDir);
	}

	// �ļ����в�����qss�ļ�
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

void RenderZMainPage::populateModelFromPath(ZQtNavigatorModel* model, const QString& path, const QModelIndex& parent)
{
	QDir dir(path);
	// ���ù���������ȡ�����ļ���Ŀ¼���������� "." �� ".."
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	// ����������Ŀ¼����
	dir.setSorting(QDir::DirsFirst | QDir::Name);

	QFileInfoList list = dir.entryInfoList();
	for (const QFileInfo& fileInfo : list)
	{
		if (fileInfo.isDir())
		{
			// �����Ŀ¼�����һ��Ŀ¼�ڵ㣬Ȼ��ݹ�����Ŀ¼
			QIcon dirIcon = this->style()->standardIcon(QStyle::SP_DirIcon);
			QModelIndex dirIndex = model->addItem(parent, fileInfo.fileName(), dirIcon);
			// �ݹ����
			populateModelFromPath(model, fileInfo.filePath(), dirIndex);
		}
		else
		{
			// ������ļ���ֱ������ļ��ڵ�
			QIcon fileIcon = this->style()->standardIcon(QStyle::SP_FileIcon);
			model->addItem(parent, fileInfo.fileName(), fileIcon);
		}
	}
}


void RenderZMainPage::connectSlots()
{
	QObject::connect(mainSideBarRight_, &SideSettingView::backgroundColorChanged,
		this, &RenderZMainPage::onSideViewBackgroundColorChanged);

	QObject::connect(leftNavigView_, &ZQtNavigatorView::button1Clicked,
		this, &RenderZMainPage::onMenuClick);

	QObject::connect(leftNavigView_, &ZQtNavigatorView::button2Clicked,
		this, &RenderZMainPage::onLoadModel);

	QObject::connect(leftNavigView_, &ZQtNavigatorView::button3Clicked,
		this, &RenderZMainPage::onMoreFunction);
}



void RenderZMainPage::onSideViewBackgroundColorChanged(const QColor& newColor)
{
	mianViewer_->get3DViewer()->setBackColor(newColor.red(),newColor.green(),newColor.blue());
	return;
}


void RenderZMainPage::onLoadModel(const QModelIndex& slotindex, const QString& path)
{
	std::string modelPathStr = MODEL_PATH;
	std::string modelToLoadPathStr = modelPathStr + path.toStdString();
	fs::path filePath(modelToLoadPathStr);
	bool fileIsValid = false;
	try {
		if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
			// ����׺�Ƿ�Ϊ .obj (���Դ�Сд)
			std::string extension = filePath.extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (extension == ".obj") {
				fileIsValid = true;
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		qDebug() << "Filesystem error:" << e.what();
		fileIsValid = false;
	}

	if (fileIsValid) {
		qDebug() << "File is valid. Loading:" << QString::fromStdString(modelToLoadPathStr);
		mianViewer_->load3DModel(modelToLoadPathStr);
	}
	else {
		qDebug() << "File is not valid or does not exist:" << QString::fromStdString(modelToLoadPathStr);
		QMessageBox::warning(
			this, 
			tr("load model fail!!"), // ��������
			tr("please check out file exsits, it must be .obj!")
			.arg(QString::fromStdString(modelToLoadPathStr))
		);
	}
}


void RenderZMainPage::onMenuClick(const QModelIndex& index)
{
	// 1. ��� index �Ƿ���Ч
	if (!index.isValid()) {
		return;
	}

	// 2. ��ȡ��ʾ�������ı�
	QString displayText = index.data(Qt::DisplayRole).toString();
	qDebug() << "Button was clicked on item at row" << index.row() << "with text:" << displayText;
}

void RenderZMainPage::onMoreFunction(const QModelIndex& index)
{
	// 1. ��� index �Ƿ���Ч
	if (!index.isValid()) {
		return;
	}

	// 2. ��ȡ��ʾ�������ı�
	QString displayText = index.data(Qt::DisplayRole).toString();
	qDebug() << "Button was clicked on item at row" << index.row() << "with text:" << displayText;
}
