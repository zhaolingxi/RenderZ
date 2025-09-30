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
#include "../../../zimgui/include/zimgui_api.h"
#include <QElapsedTimer>
#include <QTimer> 
#include "scene/zcamera.h" 
#include "zimgui_viewer_debug_panel.h" 

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

class ZQTGUI_API ZQt3DViewer : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit ZQt3DViewer(QWidget* parent);
    ~ZQt3DViewer();
    bool loadModel(const QString& path);

    void setBackColor(double red, double green,double blue);
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
    QElapsedTimer m_elapsedTimer;
    ZImGui_InputState m_imguiInputState;
    QTimer* m_animationTimer; // 【新增】用于驱动持续渲染的计时器
    bool m_imguiInitialized = false; // 【新增】标志位
private:
    QScreen* screen_{nullptr};
    int coordinateSystemWidth_ = 100; // 假设坐标系控件的宽度
    int coordinateSystemHeight_ = 100; // 假设坐标系控件的高度

    double rotate_y_ = 0;
    double rotate_x_ = 0;
    double rotate_z_ = 0;

    ZCamera camera_;
    QPoint lastMousePos_;

    // 【新增】 ImGui调试面板相关的成员
    zimgui::ZIMGUIViewportDebugPanel m_debugPanel;
    zimgui::ViewerDebugState m_debugState;
    double red_ = 0.75;
    double green_ = 0.75;
    double blue_ = 0.75;
};


enum class ViewerType :int64_t { EImageType = 0, EVideoType, EModelType, ECavasType, EOpenGLType ,EIMGUIType};
class ZQTGUI_API ZQtViewer : public QFrame
{
    Q_OBJECT
public:
    explicit ZQtViewer(QWidget* parent =nullptr, ViewerType iViewerType= ViewerType::EImageType);
    virtual~ZQtViewer()=default;

    void setViewerType(ViewerType itype) { viewerType_=itype; };
    ViewerType getViewerType() { return viewerType_; };
    bool load3DModel(std::string model_path);
    ZQt3DViewer* get3DViewer();
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