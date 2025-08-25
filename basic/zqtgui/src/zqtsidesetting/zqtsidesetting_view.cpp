#include "zqtsidesetting/zqtsidesetting_view.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>
#include <QSpacerItem>

SideSettingView::SideSettingView(QWidget* parent)
    : QWidget(parent), m_backgroundColor(Qt::darkGray) // 默认背景色
{
    setupUi();
    setMaximumWidth(400); // 限制最大宽度
}

void SideSettingView::setupUi() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 移除边距，使其填满

    // 创建滚动区域，确保内容过多时可以滚动
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // 允许内部控件自动调整大小
    scrollArea->setFrameShape(QFrame::NoFrame); // 移除边框

    scrollAreaWidgetContents = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaWidgetContents);

    // --- 1. 视图设置 Group ---
    viewSettingsGroup = new QGroupBox(u8"视图设置", this);
    viewSettingsLayout = new QFormLayout(viewSettingsGroup);

    // 背景颜色
    backgroundColorButton = new QPushButton(this);
    backgroundColorButton->setFlat(true); //扁平化，看起来更像一个色块
    backgroundColorButton->setAutoFillBackground(true);
    // 更新按钮颜色
    QPalette pal = backgroundColorButton->palette();
    pal.setColor(QPalette::Button, m_backgroundColor);
    backgroundColorButton->setPalette(pal);

    connect(backgroundColorButton, &QPushButton::clicked, this, &SideSettingView::onBackgroundColorButtonClicked);
    viewSettingsLayout->addRow(u8"背景颜色:", backgroundColorButton);

    // 显示网格
    showGridCheckBox = new QCheckBox(u8"显示地面网格", this);
    showGridCheckBox->setChecked(true);
    connect(showGridCheckBox, &QCheckBox::toggled, this, &SideSettingView::gridVisibleChanged);
    viewSettingsLayout->addRow(showGridCheckBox);

    // 相机移动速度
    cameraMoveSpeedSlider = new QSlider(Qt::Horizontal, this);
    cameraMoveSpeedSlider->setRange(1, 100);
    cameraMoveSpeedSlider->setValue(50);
    connect(cameraMoveSpeedSlider, &QSlider::valueChanged, this, &SideSettingView::cameraMoveSpeedChanged);
    viewSettingsLayout->addRow(u8"相机移动速度:", cameraMoveSpeedSlider);

    // 相机旋转速度
    cameraRotateSpeedSlider = new QSlider(Qt::Horizontal, this);
    cameraRotateSpeedSlider->setRange(1, 100);
    cameraRotateSpeedSlider->setValue(50);
    connect(cameraRotateSpeedSlider, &QSlider::valueChanged, this, &SideSettingView::cameraRotateSpeedChanged);
    viewSettingsLayout->addRow(u8"相机旋转速度:", cameraRotateSpeedSlider);

    // 视野 (FOV)
    fovSpinBox = new QDoubleSpinBox(this);
    fovSpinBox->setRange(20.0, 120.0);
    fovSpinBox->setValue(60.0);
    fovSpinBox->setSuffix(" °");
    connect(fovSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SideSettingView::fieldOfViewChanged);
    viewSettingsLayout->addRow(u8"视野 (FOV):", fovSpinBox);

    scrollAreaLayout->addWidget(viewSettingsGroup);

    // --- 2. 性能与渲染 Group ---
    performanceGroup = new QGroupBox(u8"性能与渲染", this);
    performanceLayout = new QFormLayout(performanceGroup);

    // V-Sync
    vsyncCheckBox = new QCheckBox(u8"启用垂直同步", this);
    connect(vsyncCheckBox, &QCheckBox::toggled, this, &SideSettingView::vsyncChanged);
    performanceLayout->addRow(vsyncCheckBox);

    // 纹理质量
    textureQualityComboBox = new QComboBox(this);
    textureQualityComboBox->addItems({ u8"低", u8"中", u8"高" });
    textureQualityComboBox->setCurrentIndex(2); // 默认高
    connect(textureQualityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::textureQualityChanged);
    performanceLayout->addRow(u8"纹理质量:", textureQualityComboBox);

    // 抗锯齿
    antiAliasingComboBox = new QComboBox(this);
    antiAliasingComboBox->addItems({ u8"无", "FXAA", "MSAA x4" });
    antiAliasingComboBox->setCurrentIndex(1); // 默认 FXAA
    connect(antiAliasingComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::antiAliasingChanged);
    performanceLayout->addRow(u8"抗锯齿:", antiAliasingComboBox);

    scrollAreaLayout->addWidget(performanceGroup);

    // --- 3. 应用设置 Group ---
    appSettingsGroup = new QGroupBox(u8"应用设置", this);
    appSettingsLayout = new QFormLayout(appSettingsGroup);

    // UI 主题
    themeComboBox = new QComboBox(this);
    themeComboBox->addItems({ u8"暗色主题", u8"亮色主题" });
    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::themeChanged);
    appSettingsLayout->addRow(u8"UI 主题:", themeComboBox);

    // 操作按钮
    resetSettingsButton = new QPushButton(u8"恢复默认设置", this);
    aboutButton = new QPushButton(u8"关于", this);
    connect(aboutButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, u8"关于", u8"模型查看与管理器 zrender_v1.0\n由 ZQTEngine 驱动");
        });

    // 将按钮放在一个水平布局中，并排显示
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetSettingsButton);
    buttonLayout->addWidget(aboutButton);
    appSettingsLayout->addRow(buttonLayout);

    scrollAreaLayout->addWidget(appSettingsGroup);

    // 添加一个弹簧，将所有内容推到顶部
    scrollAreaLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 完成布局
    scrollArea->setWidget(scrollAreaWidgetContents);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

// 背景颜色按钮被点击时的槽函数
void SideSettingView::onBackgroundColorButtonClicked() {
    QColor newColor = QColorDialog::getColor(m_backgroundColor, this, u8"选择背景颜色");
    if (newColor.isValid()) {
        m_backgroundColor = newColor;

        // 更新按钮的显示颜色
        QPalette pal = backgroundColorButton->palette();
        pal.setColor(QPalette::Button, m_backgroundColor);
        backgroundColorButton->setPalette(pal);

        // 发出信号，通知外部背景色已改变
        emit backgroundColorChanged(m_backgroundColor);
    }
}