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
    makeCurrent();
    // 先关闭依赖于OpenGL上下文的后端
    if (m_imguiInitialized) {
        ZImGui_Shutdown_OpenGL();
    }
    // 然后关闭与OpenGL无关的核心上下文
    ZImGui_Shutdown();
    doneCurrent();
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


    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glClearColor(1, 1, 1, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
    glEnable(GL_NORMALIZE);


    red_ = 0.75;
    green_ = 0.75;
    blue_ = 0.75;


    int ratio = devicePixelRatio();
    camera_.reset();


    glPushMatrix();

    // 【修改】将 coordinateSystem 的初始化放在你的GL设置之后
    if (coordinateSystem_ != nullptr) {
        coordinateSystem_->initializeGL();
    }

    // 【新增】在所有GL设置完成后，初始化ImGui
    m_elapsedTimer.start();
}
void ZQt3DViewer::resizeGL(int w, int h)
{
 
    if (h == 0) h = 1;

    // 【核心修改】使用相机来设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    Eigen::Matrix4f projectionMatrix = camera_.getProjectionMatrix(w, h);
    // 使用 .data() 将 Eigen 矩阵加载到 OpenGL
    glLoadMatrixf(projectionMatrix.data());

    // 【重要】切换回模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 坐标系和视口设置保持不变
    glViewport(0, 0, w, h);
    coordinateSystem_->resizeGL(coordinateSystemWidth_, coordinateSystemHeight_);
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);
}

void ZQt3DViewer::paintGL()
{
    // --- 第1步: UI 构建 ---
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    const qreal retinaScale = devicePixelRatioF();
    ZImGui_NewFrame(width() * retinaScale, height() * retinaScale, deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // 【核心修复】使用 "宿主窗口" 模式来创建全屏Dockspace
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground; // 直接设置宿主窗口无背景

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpaceHost", nullptr, host_window_flags);

        ImGui::PopStyleVar(3);

        // 创建DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }

    // 现在，我们可以创建我们自己的窗口，它们会自动停靠到上面创建的DockSpace中
    ImGui::Begin("Controls");
    ImGui::Text("Your 3D Viewer Controls");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    //ImGui::ShowDemoWindow();
    m_debugPanel.draw(u8"调试面板 (Debug Panel)", m_debugState);

    // --- 第2步: 清空屏幕 ---
    glViewport(0, 0, width(), height());
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- 第3步: 渲染我们自己的3D/2D场景 ---
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {
        // === A. 渲染3D主场景 ===
        glMatrixMode(GL_MODELVIEW);
        Eigen::Matrix4f viewMatrix = camera_.getViewMatrix();
        glLoadMatrixf(viewMatrix.data());

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
        // === B. 渲染坐标系 ===
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

    // --- 第4步: 渲染ImGui ---
    ZImGui_Render();
}


void ZQt3DViewer::wheelEvent(QWheelEvent* event)
{
  
    m_imguiInputState.mouse_wheel = event->angleDelta().y() / 120.0f;
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    // 【核心修改】将滚轮事件传递给相机
    float delta = event->angleDelta().y() / 120.0f;
    camera_.zoom(delta);

    coordinateSystem_->wheelEvent(event);
}

void ZQt3DViewer::mousePressEvent(QMouseEvent* event)
{
    // 【核心修复】获取设备像素比
    const qreal retinaScale = devicePixelRatioF();
    // 【核心修复】将逻辑鼠标坐标转换为物理坐标
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;

    // 【新增】更新ImGui输入状态
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = true;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = true;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = true;

    if (ImGui::GetIO().WantCaptureMouse) {
       return;
    }

    // 【核心修改】记录鼠标按下的起始位置
    lastMousePos_ = event->pos();

    coordinateSystem_->mousePressEvent(event);
}

void ZQt3DViewer::mouseMoveEvent(QMouseEvent* event)
{
    // 【核心修复】获取设备像素比
    const qreal retinaScale = devicePixelRatioF();
    // 【核心修复】将逻辑鼠标坐标转换为物理坐标
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;


    if (ImGui::GetIO().WantCaptureMouse) {
        //update();
        return;
    }

    // 【核心修改】相机交互逻辑
    QPointF delta = QPointF(event->pos()) - lastMousePos_;

    // 如果按下的是左键，则旋转
    if (event->buttons() & Qt::LeftButton) {
        camera_.rotate(delta.x(), delta.y());
    }
    // 如果按下的是右键 (或中键)，则平移
    else if (event->buttons() & Qt::RightButton) {
        camera_.pan(delta.x(), delta.y());
    }

    // 【重要】更新最后的位置以供下一帧计算
    lastMousePos_ = event->pos();

    coordinateSystem_->mouseMoveEvent(event);
}

void ZQt3DViewer::mouseReleaseEvent(QMouseEvent* event)
{
    // 【核心修复】获取设备像素比
    const qreal retinaScale = devicePixelRatioF();
    // 【核心修复】将逻辑鼠标坐标转换为物理坐标
    m_imguiInputState.mouse_x = event->x() * retinaScale;
    m_imguiInputState.mouse_y = event->y() * retinaScale;
    // 【新增】更新ImGui输入状态
    if (event->button() == Qt::LeftButton) m_imguiInputState.mouse_down[0] = false;
    if (event->button() == Qt::RightButton) m_imguiInputState.mouse_down[1] = false;
    if (event->button() == Qt::MiddleButton) m_imguiInputState.mouse_down[2] = false;

    if (ImGui::GetIO().WantCaptureMouse) {

    }

    coordinateSystem_->mouseReleaseEvent(event);
}

bool ZQt3DViewer::loadModel(const QString& path)
{
    // 如果已经有一个模型，先释放它
    if (objLoader_ != nullptr) {
        delete objLoader_;
        objLoader_ = nullptr;
    }

    // 创建加载器实例来加载文件
    objLoader_ = new zrender::ZObjModelLoader(path.toStdString());

    // 检查加载是否成功（例如，是否有顶点数据）
    if (objLoader_ == nullptr || objLoader_->getVertices().empty()) {
        qDebug() << "Failed to load model or model is empty:" << path;
        delete objLoader_;
        objLoader_ = nullptr;
        return false;
    }

    qDebug() << "Model loaded successfully:" << path;
    qDebug() << "Vertices:" << objLoader_->getVertices().size() << ", Faces:" << objLoader_->getFacets().size();


    // 请求重绘以显示新加载的模型
    update();
    return true;
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

