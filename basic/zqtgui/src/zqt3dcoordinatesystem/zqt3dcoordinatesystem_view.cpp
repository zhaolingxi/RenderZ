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
    glViewport(0, 0, w, h);           //���õ�ǰ���ӿ�
    // glMatrixMode() ����ָ����ǰҪ�����ľ���  GL_MODELVIEW��ģ����ͼ��Ĭ��ֵ����GL_PROJECTION��ͶӰ����GL_TEXTURE��������GL_COLOR����ɫ��
    glMatrixMode(GL_PROJECTION);      //ѡ��ͶӰ����
    glLoadIdentity();                 //����ͶӰ���� Ϊ ��λ����

    gluPerspective(60.0, double(w) / h, 0.1, 1000.0);  //����͸��ͶӰ

    glMatrixMode(GL_MODELVIEW);       //ѡ��ģ�;���
    glLoadIdentity();                 //����ģ�;��� Ϊ ��λ����
}

void ZQt3DCoordinateSystem::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    glTranslatef(0, 0, translationZ);    // ������ͼ����
    glRotatef(rotationX, 0, -1, 0);  // ��y����ת45��
    glRotatef(rotationY, 1, 0, -1);  // ������(1,0,-1) ��ת45��

    // ������
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

    // �������ͷ  �ο��ٷ��ĵ�: https://registry.khronos.org/OpenGL-Refpages/gl4/
    GLUquadricObj* objCylinder = gluNewQuadric();
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder����һ����z�ᶨ���Բ����
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(90, 0, 1, 0);      // ����y��ת90�� ʹ��x�����ԭz�᷽��
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder����һ����z�ᶨ���Բ����
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glRotatef(-90, 1, 0, 0.0);   // ����x��ת90�� ʹ��y�����ԭz�᷽��
    glTranslatef(0, 0, axis_length);
    gluCylinder(objCylinder, 0.3, 0.0, 0.3, 100, 1);  // gluCylinder����һ����z�ᶨ���Բ����
    glPopMatrix();

    // ��ԭ�� - ��ɫ
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
    float scaleStep = 0.15; // ���Ų���
    if (event->angleDelta().y() > 0) {
        translationZ -= scaleStep; // �Ŵ�
    }
    else {
        translationZ += scaleStep; // ��С
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
        planeXOffset += dx * 0.01; // ��X���ƶ�
        planeYOffset += dy * 0.01; // ��Y���ƶ�
    }
    else if (event->buttons() & Qt::RightButton) {
        // ��ת
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

