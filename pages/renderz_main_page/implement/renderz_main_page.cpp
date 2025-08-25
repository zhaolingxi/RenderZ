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
	leftNavigView_ = new ZQtNavigatorView(nullptr);
	// 3. ׼������ (ʹ�ô��������ַ����б���Ϊʾ��)

	// --- ʾ����Ϊ�ض�������ͼ�� ---
	// 1. ��ȡ������ (һ����Ч��QModelIndex�����)
	std::string assertPath = ASSERT_PATH;
	std::string icon_path = assertPath + "image/buttons/sharp_fullscreen_exit_black_20.png";
	QModelIndex rootIndex = QModelIndex();

	// 2. �ڸ�����Ӷ�������������ǵ�����
	QModelIndex projectA = leftNavigModel_->addItem(rootIndex, "Project A", QIcon(icon_path.c_str()));
	QModelIndex projectB = leftNavigModel_->addItem(rootIndex, "Project B", QIcon(icon_path.c_str()));

	// 3. �� "Project A" ���������
	QModelIndex sourceFiles = leftNavigModel_->addItem(projectA, "Source Files", QIcon(icon_path.c_str()));
	QModelIndex headerFiles = leftNavigModel_->addItem(projectA, "Header Files", QIcon(icon_path.c_str()));

	// 4. �� "Source Files" �����������
	leftNavigModel_->addItem(sourceFiles, "main.cpp", QIcon(icon_path.c_str()));
	leftNavigModel_->addItem(sourceFiles, "widget.cpp", QIcon(icon_path.c_str()));

	// 5. �� "Header Files" �����������
	leftNavigModel_->addItem(headerFiles, "widget.h", QIcon(icon_path.c_str()));

	// 6. �� "Project B" ���������
	QModelIndex documents = leftNavigModel_->addItem(projectB, "Documents", QIcon(icon_path.c_str()));
	leftNavigModel_->addItem(documents, "readme.txt", QIcon(icon_path.c_str()));

	leftNavigView_->setModel(leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);
	std::string modelPath = MODEL_PATH; modelPath += "AfricanHead/african_head.obj";
	std::cout << modelPath << std::endl;
	mianViewer_->load3DModel(modelPath);

	mainSideBarRight_ = new SideSettingView(this);

	pageViewLayoutMid->addWidget(leftNavigView_,0);
	pageViewLayoutMid->addWidget(mianViewer_,1);
	pageViewLayoutMid->addWidget(mainSideBarRight_,0);
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
	// ... (Viewer����) ...

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
	mainLayout_ = new QGridLayout(this);
	mainSideBarLeft_ = new QWidget(this);
	mainSideBarRight_ = new QWidget(this);
	pageViewLayout_ = new QVBoxLayout();
	pageViewLayoutTop_ = new QHBoxLayout();
	pageViewLayoutMid_ = new QHBoxLayout();
	//QSplitter* midSplitter = createMidSplitter();
	pageViewLayoutBtm_ = new QHBoxLayout();

	createLayoutTop(pageViewLayoutTop_);
	createLayoutMid(pageViewLayoutMid_);
	createLayoutBtm(pageViewLayoutBtm_);

	pageViewLayout_->addLayout(pageViewLayoutTop_);
	pageViewLayout_->addLayout(pageViewLayoutMid_);
	//pageViewLayout_->addWidget(midSplitter);
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

