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
	// 3. 准备数据 (使用带缩进的字符串列表作为示例)

	// --- 示例：为特定项设置图标 ---
	// 1. 获取根索引 (一个无效的QModelIndex代表根)
	std::string assertPath = ASSERT_PATH;
	std::string icon_path = assertPath + "image/buttons/sharp_fullscreen_exit_black_20.png";
	QModelIndex rootIndex = QModelIndex();

	// 2. 在根下添加顶级项，并保存它们的索引
	QModelIndex projectA = leftNavigModel_->addItem(rootIndex, "Project A", QIcon(icon_path.c_str()));
	QModelIndex projectB = leftNavigModel_->addItem(rootIndex, "Project B", QIcon(icon_path.c_str()));

	// 3. 在 "Project A" 下添加子项
	QModelIndex sourceFiles = leftNavigModel_->addItem(projectA, "Source Files", QIcon(icon_path.c_str()));
	QModelIndex headerFiles = leftNavigModel_->addItem(projectA, "Header Files", QIcon(icon_path.c_str()));

	// 4. 在 "Source Files" 下添加孙子项
	leftNavigModel_->addItem(sourceFiles, "main.cpp", QIcon(icon_path.c_str()));
	leftNavigModel_->addItem(sourceFiles, "widget.cpp", QIcon(icon_path.c_str()));

	// 5. 在 "Header Files" 下添加孙子项
	leftNavigModel_->addItem(headerFiles, "widget.h", QIcon(icon_path.c_str()));

	// 6. 在 "Project B" 下添加子项
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
	// 创建部件 (这部分代码不变)
	leftNavigModel_ = new ZQtNavigatorModel();
	leftNavigView_ = new ZQtNavigatorView(this); // 建议指定父对象
	// ... (模型设置) ...
	leftNavigView_->setModel(leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);
	// ... (Viewer设置) ...

	mainSideBarRight_ = new SideSettingView(this);

	// 创建一个水平方向的 QSplitter
	QSplitter* midSplitter = new QSplitter(Qt::Horizontal, this);

	// 将部件添加到 Splitter 中
	midSplitter->addWidget(leftNavigView_);
	midSplitter->addWidget(mianViewer_);
	midSplitter->addWidget(mainSideBarRight_);

	// (可选) 设置初始的相对大小比例
	// 这里设置左、中、右的比例大约为 1:4:1
	midSplitter->setSizes(QList<int>() << 150 << 600 << 150);

	// (重要) 设置中间的视图为那个会拉伸的部件
	// 当窗口大小改变时，只有中间的部件会按比例缩放
	midSplitter->setStretchFactor(1, 1); // 第二个部件 (mianViewer_) 的拉伸因子设为1
	midSplitter->setStretchFactor(0, 0); // 第一个部件的拉伸因子设为0
	midSplitter->setStretchFactor(2, 0); // 第三个部件的拉伸因子设为0

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

