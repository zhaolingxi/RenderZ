#pragma once

#include"zqtgui.h"
#include<QImage>
#include<QFrame>
#include<QMainWindow>
#include<QLabel>
#include<QScrollArea>
#include<QGridLayout>
#include<QOpenGLWidget>
ZQTGUI_NS_BEGIN
class ZQt3DCoordinateSystem;

class ZQtImageViewer : public QFrame
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

class ZQt3DViewer:QOpenGLWidget{
    Q_OBJECT
public:
    explicit ZQt3DViewer(QWidget* parent);
    ~ZQt3DViewer() { delete coordinateSystem_; };
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    ZQt3DCoordinateSystem* coordinateSystem_{ nullptr };
    int coordinateSystemWidth_ = 200; // 假设坐标系控件的宽度
    int coordinateSystemHeight_ = 200; // 假设坐标系控件的高度
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
    ViewerType viewerType_;

    ZQt3DCoordinateSystem * triDCoord_{nullptr};
};
ZQTGUI_NS_END