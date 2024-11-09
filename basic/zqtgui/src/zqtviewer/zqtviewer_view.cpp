#include "zqtviewer_view.h"
#include<QFileDialog>
#include<QScrollBar>
#include<QImageWriter>
#include<QGuiApplication>
#include<QScreen>
#include"zqt3dcoordinatesystem_view.h"
ZQTGUI_NS_BEGIN

ZQtImageViewer::ZQtImageViewer(QWidget* parent)
    : QFrame(parent), imageLabel(new QLabel("test"))
    , scrollArea(new QScrollArea)
{
    mainLayout_ = new QGridLayout(this);

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    QString testSheet = "QLabel{ color: #ed0f0f; background - color: #ed0f0f; }";
    imageLabel->setStyleSheet(testSheet);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(true);

    mainLayout_->addWidget(scrollArea);
    //setCentralWidget(scrollArea);

   // createActions();

  //  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    show();
}


ZQtViewer::ZQtViewer(QWidget* parent, ViewerType iViewerType)
{
    viewerType_ = iViewerType;
    createViewer();
}

void ZQtViewer::createViewer()
{
    mainLayout_ = new QGridLayout(this);
    if (viewerType_== ViewerType::EImageType) {
        zImageViewer_ = new ZQtImageViewer(this);
        mainLayout_->addWidget(zImageViewer_);
    }
    else if (viewerType_ == ViewerType::EOpenGLType) {
        triDCoord_ = new ZQt3DCoordinateSystem(this);
        triDCoord_->setMinimumSize(400, 400);
        mainLayout_->addWidget(triDCoord_);
    }
    show();
}
ZQTGUI_NS_END