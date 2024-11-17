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

ZQt3DViewer::ZQt3DViewer(QWidget* parent)
{
    coordinateSystem_ = new ZQt3DCoordinateSystem(this);
}

void ZQt3DViewer::initializeGL()
{
    coordinateSystem_->initializeGL();
}

void ZQt3DViewer::resizeGL(int w, int h)
{
    // 设置主视图的视口和投影矩阵
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 调整坐标系控件的位置和大小
   
    coordinateSystem_->resizeGL(coordinateSystemWidth_, coordinateSystemHeight_);

}

void ZQt3DViewer::paintGL()
{
    // 再渲染坐标系，将其放在右下角
    glViewport(width() - coordinateSystemWidth_, height() - coordinateSystemHeight_, coordinateSystemWidth_, coordinateSystemHeight_); // 假设坐标系控件大小为200x200
    coordinateSystem_->paintGL();
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

