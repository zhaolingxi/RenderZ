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


    int ratio = devicePixelRatio();
    camera_.reset();

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

    // 【重要】获取设备像素比
    const qreal dpr = devicePixelRatioF();

    // 先处理子控件，同样需要考虑DPI
    const int coord_physical_w = coordinateSystemWidth_ * dpr;
    const int coord_physical_h = coordinateSystemHeight_ * dpr;
    coordinateSystem_->resizeGL(coord_physical_w, coord_physical_h);
    // move() 使用的是逻辑坐标，所以这里不用变
    coordinateSystem_->move(width() - coordinateSystemWidth_, 0);

    // 使用相机来设置投影矩阵，相机逻辑一般与像素无关，所以 w 和 h 不用变
    glMatrixMode(GL_PROJECTION);
    Eigen::Matrix4f projectionMatrix = camera_.getProjectionMatrix(w, h);
    glLoadMatrixf(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 【核心修复】使用物理像素尺寸来设置主视口
    const int physical_w = w * dpr;
    const int physical_h = h * dpr;
    std::cout << "Logical W/H: " << w << ", " << h << std::endl;
    std::cout << "Physical W/H: " << physical_w << ", " << physical_h << " (DPR: " << dpr << ")" << std::endl;
    glViewport(0, 0, physical_w, physical_h);

}



void ZQt3DViewer::paintGL()
{
    // 【重要】获取DPI，因为ImGui需要它
    const qreal dpr = devicePixelRatioF();

    // --- 第1步: 恢复ImGui UI构建 ---
    float deltaTime = m_elapsedTimer.restart() / 1000.0f;
    if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
    ZImGui_NewFrame(width() * dpr, height() * dpr, deltaTime, &m_imguiInputState);
    m_imguiInputState.mouse_wheel = 0.0f;

    // 【恢复 DockSpace 和窗口】
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
    m_debugPanel.draw(u8"调试面板 (Debug Panel)", m_debugState);


    // --- 第2步: 清空屏幕 ---
    // 【强制校正】确保视口是物理全屏
    glViewport(0, 0, width() * dpr, height() * dpr);
    glClearColor(red_, green_, blue_, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // --- 第3步: 渲染我们自己的3D/2D场景 ---
    // glPushAttrib(GL_ALL_ATTRIB_BITS); // 暂时可以不用
    {
        // === A. 渲染3D主场景 ===
        glMatrixMode(GL_MODELVIEW);
        Eigen::Matrix4f viewMatrix = camera_.getViewMatrix();
        glLoadMatrixf(viewMatrix.data());

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        if (objLoader_) {
            // ... (你绘制模型的代码) ...
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
            const qreal dpr = devicePixelRatioF(); // 再次获取，确保安全
            const int coord_physical_w = coordinateSystemWidth_ * dpr;
            const int coord_physical_h = coordinateSystemHeight_ * dpr;

            glViewport(0, 0, coord_physical_w, coord_physical_h);
            glEnable(GL_SCISSOR_TEST);
            glScissor(0, 0, coord_physical_w, coord_physical_h);

            glClear(GL_DEPTH_BUFFER_BIT); // 只清除深度，不清除颜色
            coordinateSystem_->paintGL();
            glDisable(GL_SCISSOR_TEST);

            // 【关键】立刻恢复主视口到物理全屏
            glViewport(0, 0, width() * dpr, height() * dpr);
        }

    }
    // glPopAttrib(); // 暂时可以不用


    // --- 第4步: 恢复ImGui渲染 ---
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

void ZQt3DViewer::setBackColor(double red, double green, double blue)
{
    // 1. 更新内部状态变量
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

