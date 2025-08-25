#pragma once

#include "zqtgui.h" // 假设这是您的项目API宏

#include <QWidget>
#include <QColor>

// 前向声明所有将用到的Qt类，可以减少头文件编译依赖
class QVBoxLayout;
class QScrollArea;
class QGroupBox;
class QFormLayout;
class QPushButton;
class QCheckBox;
class QSlider;
class QComboBox;
class QDoubleSpinBox;
class QLabel;

class ZQTGUI_API SideSettingView : public QWidget {
    Q_OBJECT

public:
    explicit SideSettingView(QWidget* parent = nullptr);
    ~SideSettingView() override = default;

    // --- 信号区 ---
    // 当UI控件发生变化时，会发出这些信号
    // 您的3D渲染器或其他模块可以连接到这些信号以接收更新
signals:
    // 视图设置信号
    void backgroundColorChanged(const QColor& color);
    void gridVisibleChanged(bool visible);
    void cameraMoveSpeedChanged(int value);
    void cameraRotateSpeedChanged(int value);
    void fieldOfViewChanged(double value);

    // 性能与渲染信号
    void vsyncChanged(bool enabled);
    void textureQualityChanged(int index); // 0: Low, 1: Medium, 2: High
    void antiAliasingChanged(int index);   // 0: None, 1: FXAA, 2: MSAA x4

    // 应用设置信号
    void themeChanged(int index); // 0: Dark, 1: Light

private slots:
    // 用于处理特定逻辑的私有槽函数
    void onBackgroundColorButtonClicked();

private:
    // 初始化UI的辅助函数
    void setupUi();

    // 指向UI控件的指针
    // --- 布局 ---
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* scrollAreaWidgetContents;
    QVBoxLayout* scrollAreaLayout;

    // --- 视图设置 Group ---
    QGroupBox* viewSettingsGroup;
    QFormLayout* viewSettingsLayout;
    QPushButton* backgroundColorButton;
    QCheckBox* showGridCheckBox;
    QSlider* cameraMoveSpeedSlider;
    QSlider* cameraRotateSpeedSlider;
    QDoubleSpinBox* fovSpinBox;

    // --- 性能与渲染 Group ---
    QGroupBox* performanceGroup;
    QFormLayout* performanceLayout;
    QCheckBox* vsyncCheckBox;
    QComboBox* textureQualityComboBox;
    QComboBox* antiAliasingComboBox;

    // --- 应用设置 Group ---
    QGroupBox* appSettingsGroup;
    QFormLayout* appSettingsLayout;
    QComboBox* themeComboBox;
    QPushButton* resetSettingsButton;
    QPushButton* aboutButton;

    // 成员变量
    QColor m_backgroundColor;
};