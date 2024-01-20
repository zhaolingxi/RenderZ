#include "test_page.h"
//#include "thresholdset_model.hpp"
#include <QGraphicsDropShadowEffect>
#include <QWidgetAction>
#include <QFileDialog>

ThresholdsetPopUpWidget::ThresholdsetPopUpWidget(QWidget* parent, const QString& viewName)
	: QWidget(parent)
{

}

ThresholdsetPopUpWidget::~ThresholdsetPopUpWidget()
{

}

bool ThresholdsetPopUpWidget::createView()
{
	// 1、设置View的Model,对于View的子View调用createView前需setModel
	// 2、设置View代理(包含命令代理和窗体代理)
	//setDelegate();
	// 3、初始化页面的控件、子View和子View的Model
	initUI();
	// 4、连接当前页面窗体的信号和槽函数
	connectSlots();
	return 1;
	//return UIViewBase::createView();
}

void ThresholdsetPopUpWidget::destroyView()
{
	//UIViewBase::destroyView();
}


void ThresholdsetPopUpWidget::initUI()
{
	improtPopUpWidget = new QWidget(this);
	exportPopUpWidget = new QWidget(this);

	improtfile_ = new QLabel(tr("path"), this);
	exportfile_ = new QLabel(tr("path"), this);

	gitPathImprotButton_ = new QPushButton();
	gitPathExportButton_ = new QPushButton();
	improtOKButton_ = new QPushButton(tr("improt"));
	exportOKButton_ = new QPushButton(tr("export"));

	gitPathImprotLineEdit_ = new QLineEdit();
	gitPathExportLineEdit_ = new QLineEdit();

	improtPopUpLayout_ = new QGridLayout(improtPopUpWidget);
	exportPopUpLayout_ = new QGridLayout(exportPopUpWidget);

	mainlayout_ = new QGridLayout();

	improtPopUpLayout_->addWidget(improtfile_, 0, 0, Qt::AlignRight | Qt::AlignCenter);
	improtPopUpLayout_->addWidget(gitPathImprotLineEdit_, 0, 1, Qt::AlignRight | Qt::AlignCenter);
	improtPopUpLayout_->addWidget(gitPathImprotButton_, 0, 2, Qt::AlignLeft | Qt::AlignCenter);
	improtPopUpLayout_->addWidget(improtOKButton_, 1, 2, Qt::AlignLeft | Qt::AlignBottom);

	exportPopUpLayout_->addWidget(exportfile_, 0, 0, Qt::AlignRight | Qt::AlignCenter);
	exportPopUpLayout_->addWidget(gitPathExportLineEdit_, 0, 1, Qt::AlignRight | Qt::AlignCenter);
	exportPopUpLayout_->addWidget(gitPathExportButton_, 0, 2, Qt::AlignLeft | Qt::AlignCenter);
	exportPopUpLayout_->addWidget(exportOKButton_, 1, 2, Qt::AlignRight | Qt::AlignBottom);

	improtPopUpLayout_->setRowStretch(0, 9);
	improtPopUpLayout_->setRowStretch(1, 1);
	improtPopUpLayout_->setColumnStretch(0, 2);
	improtPopUpLayout_->setColumnStretch(1, 5);
	improtPopUpLayout_->setColumnStretch(2, 2);

	exportPopUpLayout_->setRowStretch(0, 9);
	exportPopUpLayout_->setRowStretch(1, 1);
	exportPopUpLayout_->setColumnStretch(0, 2);
	exportPopUpLayout_->setColumnStretch(1, 5);
	exportPopUpLayout_->setColumnStretch(2, 2);

	improtPopUpWidget->setObjectName("pupUpWidget");
	exportPopUpWidget->setObjectName("pupUpWidget");

	gitPathImprotButton_->setObjectName("gitPathButton");
	gitPathExportButton_->setObjectName("gitPathButton");

	gitPathImprotLineEdit_->setObjectName("capilotControlLineEdit");
	gitPathExportLineEdit_->setObjectName("capilotControlLineEdit");

	exportOKButton_->setObjectName("popUpButton");
	improtOKButton_->setObjectName("popUpButton");

	improtPopUpWidget->hide();
	exportPopUpWidget->hide();

	mainlayout_->addWidget(improtPopUpWidget, 0, 0);
	mainlayout_->addWidget(exportPopUpWidget, 0, 0);

	this->setLayout(mainlayout_);
}

void ThresholdsetPopUpWidget::connectSlots() {
	connect(gitPathImprotButton_, &QPushButton::clicked, this, &ThresholdsetPopUpWidget::gitPathImprotButtonClicked);
	connect(gitPathExportButton_, &QPushButton::clicked, this, &ThresholdsetPopUpWidget::gitPathExportButtonClicked);
}



void ThresholdsetPopUpWidget::gitPathExportButtonClicked() {
	QFileDialog filedialog(nullptr, tr("Save File"), "./", "Files(*.csv)");
	filedialog.setAcceptMode(QFileDialog::AcceptSave);
	filedialog.setViewMode(QFileDialog::List);
	filedialog.setFileMode(QFileDialog::AnyFile);
	filedialog.setLabelText(QFileDialog::Reject, QObject::tr("Cancel"));      //取消按钮文本

	filedialog.setOption(QFileDialog::DontUseNativeDialog);
	if (filedialog.exec() == QDialog::Accepted)
	{
		QStringList filePaths = filedialog.selectedFiles();
		QString filename = filePaths[0];

		if (!filename.endsWith(".csv")) {
			filename += ".csv";
		}
		gitPathExportLineEdit_->setText(filename);
	}
}

void ThresholdsetPopUpWidget::gitPathImprotButtonClicked() {
	// 使用QFileDialog选择单个文件
	QString gitPath = QFileDialog::getOpenFileName(
		nullptr, tr("select file"), "", "All Files (*csv*)");
	if (!gitPath.isEmpty()) {
		gitPathImprotLineEdit_->setText(gitPath);
	}
}