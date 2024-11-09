#include "zqt3dcoordinatesystem_view.h"
#include<QMatrix4x4>
ZQTGUI_NS_BEGIN

ZQt3DCoordinateSystem::ZQt3DCoordinateSystem(QWidget* parent)
{
    
}

void ZQt3DCoordinateSystem::initializeGL()
{
    initializeOpenGLFunctionsIfNeeded();
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void ZQt3DCoordinateSystem::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ZQt3DCoordinateSystem::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 应用旋转
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // 应用缩放
    glTranslatef(0.0f, 0.0f, translationZ);

    // 应用平面移动
    glTranslatef(planeXOffset, planeYOffset, 0.0f);

    // 绘制坐标轴
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // X轴红色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Y轴绿色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Z轴蓝色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << err;
    }

}

void ZQt3DCoordinateSystem::wheelEvent(QWheelEvent* event)
{
    float scaleStep = 0.1; // 缩放步长
    if (event->angleDelta().y() > 0) {
        translationZ -= scaleStep; // 放大
    }
    else {
        translationZ += scaleStep; // 缩小
    }
    update();
}

void ZQt3DCoordinateSystem::mousePressEvent(QMouseEvent* event)
{
    lastMousePosition = event->pos();
}

void ZQt3DCoordinateSystem::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - lastMousePosition.x();
    int dy = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        planeXOffset += dx * 0.01; // 沿X轴移动
        planeYOffset += dy * 0.01; // 沿Y轴移动
    }
    else if (event->buttons() & Qt::RightButton) {
        // 旋转
        rotationX += dy * 0.1;
        rotationY += dx * 0.1;
    }

    lastMousePosition = event->pos();
    update();
}

void ZQt3DCoordinateSystem::mouseReleaseEvent(QMouseEvent* event)
{
}

QMatrix4x4 ZQt3DCoordinateSystem::getTransformationMatrix() const
{
    QMatrix4x4 matrix;
    matrix.translate(0.0f, 0.0f, translationZ);
    matrix.rotate(QQuaternion::fromEulerAngles(rotationX, rotationY, 0));
    return matrix;
}

void ZQt3DCoordinateSystem::initializeOpenGLFunctionsIfNeeded()
{
    if (!m_functionsInitialized) {
        initializeOpenGLFunctions();
        m_functionsInitialized = true;
    }
}

ZQTGUI_NS_END

