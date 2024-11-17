#include "zqt3dcoordinatesystem/zqt3dcoordinatesystem_view.h"
#include<QMatrix4x4>'
#include <QtMath>
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
    w_ = w;
    h_ = h;
    glViewport(0, 0, w, h);           //重置当前的视口
    // glMatrixMode() 用以指定当前要操作的矩阵  GL_MODELVIEW（模型视图，默认值），GL_PROJECTION（投影），GL_TEXTURE（纹理），GL_COLOR（颜色）
    glMatrixMode(GL_PROJECTION);      //选择投影矩阵
    glLoadIdentity();                 //重置投影矩阵 为 单位矩阵

    gluPerspective(60.0, double(w) / h, 0.1, 1000.0);  //建立透视投影

    glMatrixMode(GL_MODELVIEW);       //选择模型矩阵
    glLoadIdentity();                 //重置模型矩阵 为 单位矩阵
}

void ZQt3DCoordinateSystem::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    glTranslatef(0, 0, translationZ);    // 调整绘图坐标
    glRotatef(rotationX, 0, -1, 0);  // 绕y轴旋转45度
    glRotatef(rotationY, 1, 0, -1);  // 绕向量(1,0,-1) 旋转45度

    // 坐标轴
    float axis_length = 10;
    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(axis_length, 0, 0);
    glEnd();
    glFlush();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, axis_length, 0);
    glEnd();
    glFlush();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, axis_length);
    glEnd();
    glFlush();

    // 坐标轴箭头  参考官方文档: https://registry.khronos.org/OpenGL-Refpages/gl4/
    GLUquadricObj* objCylinder = gluNewQuadric();
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder绘制一个沿z轴定向的圆柱体
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(90, 0, 1, 0);      // 沿着y轴转90度 使得x轴对着原z轴方向
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder绘制一个沿z轴定向的圆柱体
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glRotatef(-90, 1, 0, 0.0);   // 沿着x轴转90度 使得y轴对着原z轴方向
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder绘制一个沿z轴定向的圆柱体
    glPopMatrix();

    // 画原点 - 白色
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    glVertex3f(0, 0, 0);
    glEnd();
    glFlush();

    glEnd();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << err;
    }

}

void ZQt3DCoordinateSystem::wheelEvent(QWheelEvent* event)
{
    float scaleStep = 0.15; // 缩放步长
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

void ZQt3DCoordinateSystem::drawArrow(float dx, float dy, float dz, float arrowSize, float coneHeight)
{

}

ZQTGUI_NS_END

