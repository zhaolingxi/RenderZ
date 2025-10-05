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
#include<QAction>
#include<QFileDialog>
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
	QString appDir = QApplication::applicationDirPath();
	QIcon titleIcon(":/assert/image/buttons/renderz.png");
	QPushButton* titleIconbtn = new QPushButton();
	titleIconbtn->setIcon(titleIcon);
	QLabel *titleNameLab=new QLabel(tr("RenderZ_v0.1"), this);
	QLineEdit* serachLine = new QLineEdit(this);
	serachLine->setObjectName("serachLine");
	serachLine->setClearButtonEnabled(true);
	//QIcon searchIcon("assert/image/buttons/search.svg"); 
	//QAction* searchAction = new QAction(serachLine);
	//searchAction->setIcon(searchIcon);

	fileBtn_ = new QPushButton(this);
	fileBtn_->setObjectName("fileBtn");
	saveBtn_=new QPushButton(this);
	saveBtn_->setObjectName("saveBtn");
	settingBtn_ = new QPushButton(this);
	settingBtn_->setObjectName("settingBtn");
	extBtn_ = new QPushButton(this);
	extBtn_->setObjectName("extBtn");
	aboutBtn_=new QPushButton(this);
	aboutBtn_->setObjectName("aboutBtn");


	pageViewLayoutTop->addWidget(titleIconbtn);
	pageViewLayoutTop->addWidget(titleNameLab);
	//pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(fileBtn_);
	pageViewLayoutTop->addWidget(saveBtn_);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(serachLine);
	pageViewLayoutTop->addStretch();
	pageViewLayoutTop->addWidget(settingBtn_);
	pageViewLayoutTop->addWidget(extBtn_);
	pageViewLayoutTop->addWidget(aboutBtn_);
	return true;
}

bool RenderZMainPage::createLayoutMid(QHBoxLayout*& pageViewLayoutMid)
{

	//// 1. 定义要扫描的根目录
	//std::string modelPathStr = MODEL_PATH;
	//QString modelPath = QString::fromStdString(modelPathStr);

	//// 2. 获取模型的根索引
	//QModelIndex rootIndex = QModelIndex();

	//// 3. 调用递归函数开始填充模型
	//populateModelFromPath(leftNavigModel_, modelPath, rootIndex);

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
	// 创建部件 (这部分代码不变)
	leftNavigModel_ = new ZQtNavigatorModel();
	leftNavigView_ = new ZQtNavigatorView(this); // 建议指定父对象
	// ... (模型设置) ...
	leftNavigView_->setModel(leftNavigModel_);

	mianViewer_ = new ZQtViewer(this, ViewerType::EOpenGLType);

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




void RenderZMainPage::populateModelFromPath(ZQtNavigatorModel* model, const QString& path, const QModelIndex& parent)
{
	QDir dir(path);
	// 设置过滤器，获取所有文件和目录，但不包括 "." 和 ".."
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	// 按名称排序，目录优先
	dir.setSorting(QDir::DirsFirst | QDir::Name);

	QFileInfoList list = dir.entryInfoList();
	for (const QFileInfo& fileInfo : list)
	{
		if (fileInfo.isDir())
		{
			// 如果是目录，添加一个目录节点，然后递归进入该目录
			QIcon dirIcon = this->style()->standardIcon(QStyle::SP_DirIcon);
			QModelIndex dirIndex = model->addItem(parent, fileInfo.fileName(), dirIcon);
			// 递归调用
			populateModelFromPath(model, fileInfo.filePath(), dirIndex);
		}
		else
		{
			// 如果是文件，直接添加文件节点
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

	QObject::connect(fileBtn_, &QPushButton::clicked, this, &RenderZMainPage::onFileDirCheck);
}

void RenderZMainPage::onFileDirCheck()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("select model dir"));
	if (!dir.isEmpty()) {
		modelPathStr_ = dir.toStdString();
		leftNavigModel_->clear();
		QModelIndex rootIndex = QModelIndex();
		populateModelFromPath(leftNavigModel_, dir, rootIndex);
	}
}

void RenderZMainPage::onSideViewBackgroundColorChanged(const QColor& newColor)
{
	mianViewer_->get3DViewer()->setBackColor(newColor.red(),newColor.green(),newColor.blue());
	return;
}


void RenderZMainPage::onLoadModel(const QModelIndex& slotindex, const QString& path)
{
	if (modelPathStr_.empty()) {
		modelPathStr_ = MODEL_PATH;
	}
	std::string modelToLoadPathStr = modelPathStr_ +"/"+ path.toStdString();
	fs::path filePath(modelToLoadPathStr);
	bool fileIsValid = false;
	try {
		if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
			// 检查后缀是否为 .obj (忽略大小写)
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
			tr("load model fail!!"), // 弹窗标题
			tr("please check out file exsits, it must be .obj!")
			.arg(QString::fromStdString(modelToLoadPathStr))
		);
	}
}


void RenderZMainPage::onMenuClick(const QModelIndex& index)
{
	// 1. 检查 index 是否有效
	if (!index.isValid()) {
		return;
	}

	// 2. 获取显示出来的文本
	QString displayText = index.data(Qt::DisplayRole).toString();
	qDebug() << "Button was clicked on item at row" << index.row() << "with text:" << displayText;
}

void RenderZMainPage::onMoreFunction(const QModelIndex& index)
{
	// 1. 检查 index 是否有效
	if (!index.isValid()) {
		return;
	}

	// 2. 获取显示出来的文本
	QString displayText = index.data(Qt::DisplayRole).toString();
	qDebug() << "Button was clicked on item at row" << index.row() << "with text:" << displayText;
}
