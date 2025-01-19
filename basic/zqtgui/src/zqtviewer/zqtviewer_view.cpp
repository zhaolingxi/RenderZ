#include "zqtviewer/zqtviewer_view.h"
#include<QFileDialog>
#include<QScrollBar>
#include<QImageWriter>
#include<QGuiApplication>
#include<QScreen>
#include"zqt3dcoordinatesystem/zqt3dcoordinatesystem_view.h"

#ifdef WIN32

#else

#endif // (WIN32)


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
    screen_ = QGuiApplication::primaryScreen();
}

void ZQt3DViewer::initializeGL() {
    glEnable(GL_COLOR_MATERIAL);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // 处理错误
    }

    glEnable(GL_DEPTH_TEST);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        // 处理错误
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w0_ / (GLfloat)h0_, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glClearColor(1, 1, 1, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
    glEnable(GL_NORMALIZE);

    zoomF_ = 1;
    scale_ = 1;
    radius_ = 0;
    red_ = 0.75;
    green_ = 0.75;
    blue_ = 0.75;
    mag_ = 0;
    mouseHeld_ = false;
    rotationOK_ = false;
    translateOK_ = false;
    needsReset_ = false;
    cullingOK_ = false;


    int ratio = devicePixelRatio();

    w0_ = ratio * width();
    h0_ = ratio * height();
    // cam_.setAspect(w0_, h0_);
    axisOfRotation_.setX(0);
    axisOfRotation_.setY(0);
    axisOfRotation_.setZ(0);

    glPushMatrix();
    if (coordinateSystem_ != nullptr) {
        coordinateSystem_->initializeGL();
    }
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
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);
}

void ZQt3DViewer::paintGL() {
    // 渲染主视图
    glViewport(0, 0, width(), height());
    // 你的主视图渲染代码

    // 保存当前矩阵状态
    glPushMatrix(); 

    // 启用剪裁区域
    glEnable(GL_SCISSOR_TEST);

    float ratio = devicePixelRatioF();
    double scr_width = screen_->geometry().width();
    int cur_width = width() ;
  //  cur_width *= (scr_width / 1080);
    int cur_height = height();
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);

    glScissor(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);

    // 渲染坐标系，将其放在右下角
    glViewport(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->paintGL();

    // 禁用剪裁区域
    glDisable(GL_SCISSOR_TEST);

    // 恢复矩阵状态
    glPopMatrix();

    // 恢复主视图的视口设置
    glViewport(0, 0,  width(),  height() );
}


void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
    coordinateSystem_->wheelEvent(event);
    update();
}

void ZQt3DViewer::mousePressEvent(QMouseEvent* event)
{
    coordinateSystem_->mousePressEvent(event);
    update();
}

void ZQt3DViewer::mouseMoveEvent(QMouseEvent* event)
{
    coordinateSystem_->mouseMoveEvent(event);
    update();
}

void ZQt3DViewer::mouseReleaseEvent(QMouseEvent* event)
{
    coordinateSystem_->mouseReleaseEvent(event);
    update();
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
        z3DViewer_ = new ZQt3DViewer(this);
        z3DViewer_->setMinimumSize(400, 400);
        mainLayout_->addWidget(z3DViewer_);

        setMinimumSize(600, 600);
    }
    show();
}
ZQTGUI_NS_END

