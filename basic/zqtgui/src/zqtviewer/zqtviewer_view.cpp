#include "zqtviewer/zqtviewer_view.h"
#include<QFileDialog>
#include<QScrollBar>
#include<QImageWriter>
#include<QGuiApplication>
#include<QScreen>
#include"zqt3dcoordinatesystem/zqt3dcoordinatesystem_view.h"

// 【新增】包含 ImGui C++ 头文件和 Qt 事件头文件
#include "imgui.h"
#include <QMouseEvent>
#include <QWheelEvent>

#ifdef WIN32
// 【新增】解决 Windows.h 与 C++ 标准库的 min/max 宏冲突
#define NOMINMAX
#include <Windows.h>
#else
// on other platforms, you might not need this
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
    // 【新增】启用鼠标跟踪，以便ImGui可以响应悬停事件
    setMouseTracking(true);
    // 【新增】初始化ImGui输入状态，防止未定义行为
    memset(&m_imguiInputState, 0, sizeof(ZImGui_InputState));

    // 【新增】创建并配置计时器
    m_animationTimer = new QTimer(this);
    // 将计时器的 timeout 信号连接到 widget 的 update 槽
    connect(m_animationTimer, &QTimer::timeout, this, QOverload<void>::of(&ZQt3DViewer::update));
    // 启动计时器。间隔为0意味着“尽快”，从而创建一个高帧率的循环
    m_animationTimer->start(0);

    // 【重要】在这里只进行一次ImGui主上下文的初始化
    ZImGui_Init();
}

ZQt3DViewer::~ZQt3DViewer()
{
    // 必须使OpenGL上下文成为当前上下文才能清理资源
    makeCurrent();
    ZImGui_Shutdown();
    doneCurrent();

    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    ZImGui_Shutdown();
}

void ZQt3DViewer::initializeGL() {
    // 【修改】在这里初始化/重新初始化OpenGL后端
    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    ZImGui_Init_OpenGL();
    m_imguiInitialized = true; // 标记后端已初始化


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

    // 【修改】将 coordinateSystem 的初始化放在你的GL设置之后
    if (coordinateSystem_ != nullptr) {
        coordinateSystem_->initializeGL();
    }

    // 【新增】在所有GL设置完成后，初始化ImGui
    ZImGui_Init();
    m_elapsedTimer.start();
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

#if 0
void ZQt3DViewer::paintGL() {
    // 【新增】计算帧时间差并开始新的ImGui帧
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    // 防止deltaTime在调试断点等情况下变得过大
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    ZImGui_NewFrame(width(), height(), deltaTime, &m_imguiInputState);
    // 【新增】重置单次触发的输入（如滚轮），防止下一帧继续使用
    m_imguiInputState.mouse_wheel = 0.0f;

    // 【新增】定义ImGui UI界面
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Performance");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::ShowDemoWindow();

    // 你也可以随时打开ImGui的Demo窗口来查看所有功能
    //ImGui::ShowDemoWindow();

    // 【重要修复】在绘制任何内容之前，清空颜色和深度缓冲区
    glClearColor(red_, green_, blue_, 1.0f); // 使用你的背景色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // 【新增】在所有你自己的渲染结束后，渲染ImGui的绘制数据
    ZImGui_Render();
}
#endif

void ZQt3DViewer::paintGL() {
    // 1. 开始ImGui帧
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;

    ZImGui_NewFrame(width(), height(), deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // 定义ImGui UI
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Performance");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::ShowDemoWindow();

    // 2. 清空屏幕
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3. 【隔离】渲染你自己的旧版OpenGL场景
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // -----------------------------------------------------------------
    //  vvv 你自己的渲染代码 vvv
    // -----------------------------------------------------------------
    glViewport(0, 0, width(), height());

    // 渲染坐标系
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    glViewport(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->paintGL();
    glDisable(GL_SCISSOR_TEST);

    // -----------------------------------------------------------------
    //  ^^^ 你自己的渲染代码结束 ^^^
    // -----------------------------------------------------------------

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    // 4. 渲染ImGui
    ZImGui_Render();

}

void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
    // 【新增】将事件传递给ImGui
    m_imguiInputState.mouse_wheel = event->angleDelta().y() / 120.0f;

    // 【新增】如果ImGui占用了鼠标，则不处理3D场景的事件
    if (ImGui::GetIO().WantCaptureMouse) {
        update(); // 请求重绘以更新ImGui
        return;
    }

    coordinateSystem_->wheelEvent(event);
    update();
}

void ZQt3DViewer::mousePressEvent(QMouseEvent* event)
{
    // 【新增】更新ImGui输入状态
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = true;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = true;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = true;

    if (ImGui::GetIO().WantCaptureMouse) {
        update();
        return;
    }

    coordinateSystem_->mousePressEvent(event);
    update();
}

void ZQt3DViewer::mouseMoveEvent(QMouseEvent* event)
{
    // 【新增】更新ImGui输入状态
    m_imguiInputState.mouse_x = event->pos().x();
    m_imguiInputState.mouse_y = event->pos().y();

    if (ImGui::GetIO().WantCaptureMouse) {
        update();
        return;
    }

    coordinateSystem_->mouseMoveEvent(event);
    update();
}

void ZQt3DViewer::mouseReleaseEvent(QMouseEvent* event)
{
    // 【新增】更新ImGui输入状态
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = false;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = false;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = false;

    if (ImGui::GetIO().WantCaptureMouse) {
        update();
        // 此处不立即返回，允许3D场景处理鼠标释放事件
    }

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

