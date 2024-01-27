#include "renderz_main_page.h"

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
	QPushButton* pb1 = new QPushButton();
	pb1->setObjectName("testBtn");
	QPushButton* pb2 = new QPushButton();
	pb2->setObjectName("testBtn");
	QPushButton* pb3 = new QPushButton();
	pb3->setObjectName("testBtn");


	ioLayout->addWidget(pb1);
	ioLayout->addWidget(pb2);
	ioLayout->addWidget(pb3);


}
