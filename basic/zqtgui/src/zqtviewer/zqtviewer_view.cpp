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
    //connect(m_animationTimer, &QTimer::timeout, this, QOverload<void>::of(&ZQt3DViewer::update));
    connect(m_animationTimer, &QTimer::timeout, this, qOverload<>(&ZQt3DViewer::update));
    m_animationTimer->start(0);

    // ����Ҫ��������ֻ����һ��ImGui�������ĵĳ�ʼ��
    ZImGui_Init();
}

ZQt3DViewer::~ZQt3DViewer()
{
    makeCurrent();
    // �ȹر�������OpenGL�����ĵĺ��
    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    // Ȼ��ر���OpenGL�޹صĺ���������
    ZImGui_Shutdown();
    doneCurrent();
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
        // ��������
    }

    glEnable(GL_DEPTH_TEST);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        // ��������
    }


    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glClearColor(1, 1, 1, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
    glEnable(GL_NORMALIZE);


    int ratio = devicePixelRatio();
    camera_.reset();

    // ���޸ġ��� coordinateSystem �ĳ�ʼ���������GL����֮��
    if (coordinateSystem_ != nullptr) {
        coordinateSystem_->initializeGL();
    }

    // ��������������GL������ɺ󣬳�ʼ��ImGui
    m_elapsedTimer.start();
}
void ZQt3DViewer::resizeGL(int w, int h)
{
    if (h == 0) h = 1;

    // ����Ҫ����ȡ�豸���ر�
    const qreal dpr = devicePixelRatioF();

    // �ȴ����ӿؼ���ͬ����Ҫ����DPI
    const int coord_physical_w = coordinateSystemWidth_ * dpr;
    const int coord_physical_h = coordinateSystemHeight_ * dpr;
    coordinateSystem_->resizeGL(coord_physical_w, coord_physical_h);
    // move() ʹ�õ����߼����꣬�������ﲻ�ñ�
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);

    // ʹ�����������ͶӰ��������߼�һ���������޹أ����� w �� h ���ñ�
    glMatrixMode(GL_PROJECTION);
    Eigen::Matrix4f projectionMatrix = camera_.getProjectionMatrix(w, h);
    glLoadMatrixf(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // �������޸���ʹ���������سߴ����������ӿ�
    const int physical_w = w * dpr;
    const int physical_h = h * dpr;
    std::cout << "Logical W/H: " << w << ", " << h << std::endl;
    std::cout << "Physical W/H: " << physical_w << ", " << physical_h << " (DPR: " << dpr << ")" << std::endl;
    glViewport(0, 0, physical_w, physical_h);

}



void ZQt3DViewer::paintGL()
{
    // ����Ҫ����ȡDPI����ΪImGui��Ҫ��
    const qreal dpr = devicePixelRatioF();

    // --- ��1��: �ָ�ImGui UI���� ---
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    ZImGui_NewFrame(width() * dpr, height() * dpr, deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // ���ָ� DockSpace �ʹ��ڡ�
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpaceHost", nullptr, host_window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }
    //ImGui::Begin("Controls");
    //ImGui::Text("Your 3D Viewer Controls");
    //ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::End();
    m_debugPanel.draw(u8"������� (Debug Panel)", m_debugState);


    // --- ��2��: �����Ļ ---
    // ��ǿ��У����ȷ���ӿ�������ȫ��
    glViewport(0, 0, width() * dpr, height() * dpr);
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // --- ��3��: ��Ⱦ�����Լ���3D/2D���� ---
    // glPushAttrib(GL_ALL_ATTRIB_BITS); // ��ʱ���Բ���
    {
        // === A. ��Ⱦ3D������ ===
        glMatrixMode(GL_MODELVIEW);
        Eigen::Matrix4f viewMatrix = camera_.getViewMatrix();
        glLoadMatrixf(viewMatrix.data());

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        if (objLoader_) {
            // ... (�����ģ�͵Ĵ���) ...
            glPushMatrix();
            GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            const auto& vertices = objLoader_->getVertices();
            const auto& faces = objLoader_->getFacets();
            glBegin(GL_TRIANGLES);
            for (const auto& face : faces) {
                const auto& indices = face.getVertexIndices();
                if (indices.size() >= 3) {
                    const zmath::ZVertex& v1 = vertices[indices[0] - 1];
                    const zmath::ZVertex& v2 = vertices[indices[1] - 1];
                    const zmath::ZVertex& v3 = vertices[indices[2] - 1];
                    QVector3D edge1(v2.X() - v1.X(), v2.Y() - v1.Y(), v2.Z() - v1.Z());
                    QVector3D edge2(v3.X() - v1.X(), v3.Y() - v1.Y(), v3.Z() - v1.Z());
                    QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();
                    glNormal3f(normal.x(), normal.y(), normal.z());
                    glVertex3f(v1.X(), v1.Y(), v1.Z());
                    glVertex3f(v2.X(), v2.Y(), v2.Z());
                    glVertex3f(v3.X(), v3.Y(), v3.Z());
                }
            }
            glEnd();
            glPopMatrix();
        }

        // === B. ��Ⱦ����ϵ === 
        if (coordinateSystem_) {
            const qreal dpr = devicePixelRatioF(); // �ٴλ�ȡ��ȷ����ȫ
            const int coord_physical_w = coordinateSystemWidth_ * dpr;
            const int coord_physical_h = coordinateSystemHeight_ * dpr;

            glViewport(0, 0, coord_physical_w, coord_physical_h);
            glEnable(GL_SCISSOR_TEST);
            glScissor(0, 0, coord_physical_w, coord_physical_h);

            glClear(GL_DEPTH_BUFFER_BIT); // ֻ�����ȣ��������ɫ
            coordinateSystem_->paintGL();
            glDisable(GL_SCISSOR_TEST);

            // ���ؼ������ָ̻����ӿڵ�����ȫ��
            glViewport(0, 0, width() * dpr, height() * dpr);
        }

    }
    // glPopAttrib(); // ��ʱ���Բ���


    // --- ��4��: �ָ�ImGui��Ⱦ ---
    ZImGui_Render();
}

void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
  
    m_imguiInputState.mouse_wheel = event->angleDelta().y() / 120.0f;
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    // �������޸ġ��������¼����ݸ����
    float delta = event->angleDelta().y() / 120.0f;
    camera_.zoom(delta);

    coordinateSystem_->wheelEvent(event);
}

void ZQt3DViewer::mousePressEvent(QMouseEvent* event)
{
    // �������޸�����ȡ�豸���ر�
    const qreal retinaScale = devicePixelRatioF();
    // �������޸������߼��������ת��Ϊ��������
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;

    // ������������ImGui����״̬
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = true;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = true;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = true;

    if (ImGui::GetIO().WantCaptureMouse) {
       return;
    }

    // �������޸ġ���¼��갴�µ���ʼλ��
    lastMousePos_ = event->pos();

    coordinateSystem_->mousePressEvent(event);
}

void ZQt3DViewer::mouseMoveEvent(QMouseEvent* event)
{
    // �������޸�����ȡ�豸���ر�
    const qreal retinaScale = devicePixelRatioF();
    // �������޸������߼��������ת��Ϊ��������
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;


    if (ImGui::GetIO().WantCaptureMouse) {
        //update();
        return;
    }

    // �������޸ġ���������߼�
    QPointF delta = QPointF(event->pos()) - lastMousePos_;

    // ������µ������������ת
    if (event->buttons() & Qt::LeftButton) {
        camera_.rotate(delta.x(), delta.y());
    }
    // ������µ����Ҽ� (���м�)����ƽ��
    else if (event->buttons() & Qt::RightButton) {
        camera_.pan(delta.x(), delta.y());
    }

    // ����Ҫ����������λ���Թ���һ֡����
    lastMousePos_ = event->pos();

    coordinateSystem_->mouseMoveEvent(event);
}

void ZQt3DViewer::mouseReleaseEvent(QMouseEvent* event)
{
    // �������޸�����ȡ�豸���ر�
    const qreal retinaScale = devicePixelRatioF();
    // �������޸������߼��������ת��Ϊ��������
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;
    // ������������ImGui����״̬
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = false;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = false;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = false;

    if (ImGui::GetIO().WantCaptureMouse) {

    }

    coordinateSystem_->mouseReleaseEvent(event);
}

bool ZQt3DViewer::loadModel(const QString& path)
{
    // ����Ѿ���һ��ģ�ͣ����ͷ���
    if (objLoader_ != nullptr) {
        delete objLoader_;
        objLoader_ = nullptr;
    }

    // ����������ʵ���������ļ�
    objLoader_ = new zrender::ZObjModelLoader(path.toStdString());

    // �������Ƿ�ɹ������磬�Ƿ��ж������ݣ�
    if (objLoader_ == nullptr || objLoader_->getVertices().empty()) {
        qDebug() << "Failed to load model or model is empty:" << path;
        delete objLoader_;
        objLoader_ = nullptr;
        return false;
    }

    qDebug() << "Model loaded successfully:" << path;
    qDebug() << "Vertices:" << objLoader_->getVertices().size() << ", Faces:" << objLoader_->getFacets().size();


    // �����ػ�����ʾ�¼��ص�ģ��
    update();
    return true;
}

void ZQt3DViewer::setBackColor(double red, double green, double blue)
{
    // 1. �����ڲ�״̬����
    red_ = red;
    green_ = green;
    blue_ = blue;

    update();
}

ZQtViewer::ZQtViewer(QWidget* parent, ViewerType iViewerType)
{
    viewerType_ = iViewerType;
    createViewer();
}

bool ZQtViewer::load3DModel(std::string model_path)
{
    if (z3DViewer_) {
        z3DViewer_->loadModel(model_path.c_str());
        return true;
    }
    return false;
}

ZQt3DViewer* ZQtViewer::get3DViewer() {
    return z3DViewer_;
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

