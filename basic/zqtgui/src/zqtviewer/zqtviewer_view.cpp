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
        // �������
    }

    glEnable(GL_DEPTH_TEST);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        // �������
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
    m_elapsedTimer.start();
}
void ZQt3DViewer::resizeGL(int w, int h)
{
    // ��ֹhΪ0�����³���0�Ĵ���
    if (h == 0) h = 1;

    // �������޸���ͶӰ��������Ӧ�������
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

    // ����Ҫ���л���ģ����ͼ����Ϊ paintGL() ��׼��
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // ÿ�ε�����С������ģ����ͼ����

    // ��������ϵ�ؼ���λ�úʹ�С (�ⲿ�ֱ��ֲ���)
    coordinateSystem_->resizeGL(coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);
}

#if 1
void ZQt3DViewer::paintGL()
{
    // --- ��1��: UI ���� ---
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    const qreal retinaScale = devicePixelRatioF();
    ZImGui_NewFrame(width() * retinaScale, height() * retinaScale, deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // �������޸���ʹ�� "��������" ģʽ������ȫ��Dockspace
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground; // ֱ���������������ޱ���

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpaceHost", nullptr, host_window_flags);

        ImGui::PopStyleVar(3);

        // ����DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }

    // ���ڣ����ǿ��Դ��������Լ��Ĵ��ڣ����ǻ��Զ�ͣ�������洴����DockSpace��
    ImGui::Begin("Controls");
    ImGui::Text("Your 3D Viewer Controls");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::ShowDemoWindow();

    // --- ��2��: �����Ļ ---
    glViewport(0, 0, width(), height());
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- ��3��: ��Ⱦ�����Լ���3D/2D���� ---
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {
        // === A. ��Ⱦ3D������ ===
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        if (objLoader_) {
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
            glViewport(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
            glEnable(GL_SCISSOR_TEST);
            glScissor(0, 0, coordinateSystemWidth_, coordinateSystemHeight_);
            glClear(GL_DEPTH_BUFFER_BIT);
            coordinateSystem_->paintGL();
            glDisable(GL_SCISSOR_TEST);
        }
    }
    glPopAttrib();

    // --- ��4��: ��ȾImGui ---
    ZImGui_Render();
}
#else
void ZQt3DViewer::paintGL() {
    // 1. ��ʼImGui֡
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;

    // �������޸�����ȡ�豸���ر�
    const qreal retinaScale = devicePixelRatioF();

    // �������޸������߼��ߴ�ת��Ϊ����ߴ紫�ݸ�ImGui
    ZImGui_NewFrame(width() * retinaScale, height() * retinaScale, deltaTime, &m_imguiInputState);
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
    glDisable(GL_LIGHTING);
    if (objLoader_) {
        // ����ģ����ɫ
        glColor3f(0.6f, 0.6f, 0.6f);

        // ��ȡ����ͷ�������
        const auto& vertices = objLoader_->getVertices();
        const auto& normals = objLoader_->getNormals();
        const auto& faces = objLoader_->getFacets();

        // ����������Ƭ���л���
        for (const auto& face : faces) {
            // ������Ƭ��������ѡ��ͼԪ���ͣ�ͨ���������Σ�
            // Ϊ�˼���������Ǽ������Ƕ���������
            if (face.getVertexIndices().size() >= 3) {
                glBegin(GL_TRIANGLES);

                // ������Ƭ��ÿ������
                for (size_t i = 0; i < face.getVertexIndices().size(); ++i) {
                    // OBJ �ļ������� 1 ��ʼ��C++ vector �� 0 ��ʼ��������Ҫ -1
                    int vertexIndex = face.getVertexIndices()[i] - 1;

                    // ���÷��ߣ�����������ݿ��ã�
                    // ʹ��ģ���ļ����ṩ�Ķ��㷨���Ի��ƽ��Ч��
                    if (face.getNormalIndices().size() > i) {
                        int normalIndex = face.getNormalIndices()[i] - 1;
                        if (normalIndex >= 0 && normalIndex < normals.size()) {
                            const auto& normal = normals[normalIndex];
                            glNormal3f(normal.X(), normal.Y(), normal.Z());
                        }
                    }

                    // ���ö���λ��
                    if (vertexIndex >= 0 && vertexIndex < vertices.size()) {
                        const auto& vertex = vertices[vertexIndex];
                        glVertex3f(vertex.X(), vertex.Y(), vertex.Z());
                    }
                }
                glEnd();
            }
        }
    }
    glEnable(GL_LIGHTING);

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
#endif

void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
    // �����������¼����ݸ�ImGui
    m_imguiInputState.mouse_wheel = event->angleDelta().y() / 120.0f;

    // �����������ImGuiռ������꣬�򲻴���3D�������¼�
    if (ImGui::GetIO().WantCaptureMouse) {
        //update(); // �����ػ��Ը���ImGui
        return;
    }

    coordinateSystem_->wheelEvent(event);
    //update();
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
       // update();
        return;
    }

    coordinateSystem_->mousePressEvent(event);
    //update();
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

    coordinateSystem_->mouseMoveEvent(event);
    //update();
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
        //update();
        // �˴����������أ�����3D������������ͷ��¼�
    }

    coordinateSystem_->mouseReleaseEvent(event);
    //update();
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
        z3DViewer_->loadModel("D:/Code/work/renderz/RenderZ/model/AfricanHead/african_head.obj");
    }
    show();
}
ZQTGUI_NS_END

