#include "zqtviewer/zqtviewer_view.h"
#include<QFileDialog>
#include<QScrollBar>
#include<QImageWriter>
#include<QGuiApplication>
#include<QScreen>
#include"zqt3dcoordinatesystem/zqt3dcoordinatesystem_view.h"

// ������������ ImGui C++ ͷ�ļ��� Qt �¼�ͷ�ļ�
#include "imgui.h"
#include <QMouseEvent>
#include <QWheelEvent>

#ifdef WIN32
// ����������� Windows.h �� C++ ��׼��� min/max ���ͻ
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
    // �����������������٣��Ա�ImGui������Ӧ��ͣ�¼�
    setMouseTracking(true);
    // ����������ʼ��ImGui����״̬����ֹδ������Ϊ
    memset(&m_imguiInputState, 0, sizeof(ZImGui_InputState));

    // �����������������ü�ʱ��
    m_animationTimer = new QTimer(this);
    // ����ʱ���� timeout �ź����ӵ� widget �� update ��
    connect(m_animationTimer, &QTimer::timeout, this, QOverload<void>::of(&ZQt3DViewer::update));
    // ������ʱ�������Ϊ0��ζ�š����족���Ӷ�����һ����֡�ʵ�ѭ��
    m_animationTimer->start(0);

    // ����Ҫ��������ֻ����һ��ImGui�������ĵĳ�ʼ��
    ZImGui_Init();
}

ZQt3DViewer::~ZQt3DViewer()
{
    // ����ʹOpenGL�����ĳ�Ϊ��ǰ�����Ĳ���������Դ
    makeCurrent();
    ZImGui_Shutdown();
    doneCurrent();

    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    ZImGui_Shutdown();
}

void ZQt3DViewer::initializeGL() {
    // ���޸ġ��������ʼ��/���³�ʼ��OpenGL���
    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    ZImGui_Init_OpenGL();
    m_imguiInitialized = true; // ��Ǻ���ѳ�ʼ��


    glEnable(GL_COLOR_MATERIAL);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // �������
    }

    glEnable(GL_DEPTH_TEST);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        // �������
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

    // ���޸ġ��� coordinateSystem �ĳ�ʼ���������GL����֮��
    if (coordinateSystem_ != nullptr) {
        coordinateSystem_->initializeGL();
    }

    // ��������������GL������ɺ󣬳�ʼ��ImGui
    ZImGui_Init();
    m_elapsedTimer.start();
}
void ZQt3DViewer::resizeGL(int w, int h)
{
    // ��������ͼ���ӿں�ͶӰ����
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ��������ϵ�ؼ���λ�úʹ�С
   
    coordinateSystem_->resizeGL(coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);
}

#if 0
void ZQt3DViewer::paintGL() {
    // ������������֡ʱ����ʼ�µ�ImGui֡
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    // ��ֹdeltaTime�ڵ��Զϵ������±�ù���
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    ZImGui_NewFrame(width(), height(), deltaTime, &m_imguiInputState);
    // �����������õ��δ��������루����֣�����ֹ��һ֡����ʹ��
    m_imguiInputState.mouse_wheel = 0.0f;

    // ������������ImGui UI����
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Performance");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::ShowDemoWindow();

    // ��Ҳ������ʱ��ImGui��Demo�������鿴���й���
    //ImGui::ShowDemoWindow();

    // ����Ҫ�޸����ڻ����κ�����֮ǰ�������ɫ����Ȼ�����
    glClearColor(red_, green_, blue_, 1.0f); // ʹ����ı���ɫ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ��Ⱦ����ͼ
    glViewport(0, 0, width(), height());
    // �������ͼ��Ⱦ����

    // ���浱ǰ����״̬
    glPushMatrix(); 

    // ���ü�������
    glEnable(GL_SCISSOR_TEST);

    float ratio = devicePixelRatioF();
    double scr_width = screen_->geometry().width();
    int cur_width = width() ;
  //  cur_width *= (scr_width / 1080);
    int cur_height = height();
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);

    glScissor(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);

    // ��Ⱦ����ϵ������������½�
    glViewport(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->paintGL();

    // ���ü�������
    glDisable(GL_SCISSOR_TEST);

    // �ָ�����״̬
    glPopMatrix();

    // �ָ�����ͼ���ӿ�����
    glViewport(0, 0,  width(),  height() );

    // �����������������Լ�����Ⱦ��������ȾImGui�Ļ�������
    ZImGui_Render();
}
#endif

void ZQt3DViewer::paintGL() {
    // 1. ��ʼImGui֡
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;

    ZImGui_NewFrame(width(), height(), deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // ����ImGui UI
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Performance");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::ShowDemoWindow();

    // 2. �����Ļ
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3. �����롿��Ⱦ���Լ��ľɰ�OpenGL����
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // -----------------------------------------------------------------
    //  vvv ���Լ�����Ⱦ���� vvv
    // -----------------------------------------------------------------
    glViewport(0, 0, width(), height());

    // ��Ⱦ����ϵ
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    glViewport(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->paintGL();
    glDisable(GL_SCISSOR_TEST);

    // -----------------------------------------------------------------
    //  ^^^ ���Լ�����Ⱦ������� ^^^
    // -----------------------------------------------------------------

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    // 4. ��ȾImGui
    ZImGui_Render();

}

void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
    // �����������¼����ݸ�ImGui
    m_imguiInputState.mouse_wheel = event->angleDelta().y() / 120.0f;

    // �����������ImGuiռ������꣬�򲻴���3D�������¼�
    if (ImGui::GetIO().WantCaptureMouse) {
        update(); // �����ػ��Ը���ImGui
        return;
    }

    coordinateSystem_->wheelEvent(event);
    update();
}

void ZQt3DViewer::mousePressEvent(QMouseEvent* event)
{
    // ������������ImGui����״̬
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
    // ������������ImGui����״̬
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
    // ������������ImGui����״̬
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = false;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = false;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = false;

    if (ImGui::GetIO().WantCaptureMouse) {
        update();
        // �˴����������أ�����3D������������ͷ��¼�
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

