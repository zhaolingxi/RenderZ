#pragma once

#include"zqtgui.h"
#include<QImage>
#include<QFrame>
#include<QMainWindow>
#include<QLabel>
#include<QScrollArea>
#include<QGridLayout>
#include<QOpenGLWidget>
#include<QQuaternion>
#include"model_loader/zobj_model_loader.h"
ZQTGUI_NS_BEGIN
class ZQt3DCoordinateSystem;

class ZQTGUI_API ZQtImageViewer : public QFrame
{
    Q_OBJECT

public:
    ZQtImageViewer(QWidget* parent = nullptr);
    bool loadFile(const QString&);

private slots:
    //void open();
    //void saveAs();
    //void print();
    //void copy();
    //void paste();
    //void zoomIn();
    //void zoomOut();
    //void normalSize();
    //void fitToWindow();
    //void about();

private:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString& fileName);
    void setImage(const QImage& newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar* scrollBar, double factor);
    QGridLayout* mainLayout_{ nullptr };


    QImage image;
    QLabel* imageLabel;
    QScrollArea* scrollArea;
    double scaleFactor = 1;

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printer;
#endif

    QAction* saveAsAct;
    QAction* printAct;
    QAction* copyAct;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* normalSizeAct;
    QAction* fitToWindowAct;
};

class ZQTGUI_API ZQt3DViewer:public QOpenGLWidget{
    Q_OBJECT
public:
    explicit ZQt3DViewer(QWidget* parent);
    ~ZQt3DViewer() { delete coordinateSystem_; };
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    ZQt3DCoordinateSystem* coordinateSystem_{ nullptr };
    zrender::ZObjModelLoader* objLoader_{ nullptr };
private:
    int coordinateSystemWidth_ = 200; // 假设坐标系控件的宽度
    int coordinateSystemHeight_ = 200; // 假设坐标系控件的高度

    double rotate_y_ = 0;
    double rotate_x_ = 0;
    double rotate_z_ = 0;

    QQuaternion currQ;
    /* Frustrum Things */
    float radius;
    float fdist;
    double dNear;
    double dFar;
    double viewAngle;
    float w0;
    float h0;
    /* User Control */
   // camera cam;
    /* Rotation */
    bool mouseHeld;
    bool rotationOK;
    /* Culling */
    bool cullingOK;
    bool translateOK;
    bool scaleOK;
    /* Zoom */
    bool zoomOK;
    float zoomF;
    double scale;
    /* Color Pick */
    double red, green, blue;
    bool needsReset;
    QVector3D axisOfRotation;
    int x, y, dx, dy, x0, y0;
    int prevPos[2];
    float mag;
};

enum class ViewerType :int64_t { EImageType = 0, EVideoType, EModelType, ECavasType, EOpenGLType };
class ZQTGUI_API ZQtViewer : public QFrame
{
    Q_OBJECT
public:
    explicit ZQtViewer(QWidget* parent =nullptr, ViewerType iViewerType= ViewerType::EImageType);
    virtual~ZQtViewer()=default;

    void setViewerType(ViewerType itype) { viewerType_=itype; };
    ViewerType getViewerType() { return viewerType_; };
protected:
    void createViewer();

private:
    QGridLayout* mainLayout_{ nullptr };

    ZQtImageViewer* zImageViewer_{ nullptr };
    ZQt3DViewer* z3DViewer_{ nullptr };
    ZQt3DCoordinateSystem * coordinateSystem_{nullptr};
    ViewerType viewerType_{ ViewerType::EImageType };

};
ZQTGUI_NS_END