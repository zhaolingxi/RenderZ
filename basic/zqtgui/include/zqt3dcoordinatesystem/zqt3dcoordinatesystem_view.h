#pragma once

#include"zqtgui.h"
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_5_Core>
#include <GL/glu.h>
ZQTGUI_NS_BEGIN

class ZQTGUI_API ZQt3DCoordinateSystem : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT

public:
    explicit ZQt3DCoordinateSystem(QWidget* parent = nullptr);


    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    // 获取当前的坐标变换矩阵
    QMatrix4x4 getTransformationMatrix() const;

    void drawArrow(float dx, float dy, float dz, float arrowSize, float coneHeight);

private:
    void initializeOpenGLFunctionsIfNeeded();
    bool m_functionsInitialized = false;
private:
    QPoint lastMousePosition;
    float rotationX{45.0f};
    float rotationY{ 45.0f };
    float translationZ{ -30.0f };
    float planeXOffset{ 0.0f };
    float planeYOffset{ 0.0f };
};
ZQTGUI_NS_END