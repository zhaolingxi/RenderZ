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
    : QWidget(parent), m_backgroundColor(Qt::darkGray) // Ĭ�ϱ���ɫ
{
    setupUi();
    setMaximumWidth(400); // ���������
}

void SideSettingView::setupUi() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // �Ƴ��߾࣬ʹ������

    // ������������ȷ�����ݹ���ʱ���Թ���
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // �����ڲ��ؼ��Զ�������С
    scrollArea->setFrameShape(QFrame::NoFrame); // �Ƴ��߿�

    scrollAreaWidgetContents = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaWidgetContents);

    // --- 1. ��ͼ���� Group ---
    viewSettingsGroup = new QGroupBox(u8"��ͼ����", this);
    viewSettingsLayout = new QFormLayout(viewSettingsGroup);

    // ������ɫ
    backgroundColorButton = new QPushButton(this);
    backgroundColorButton->setFlat(true); //��ƽ��������������һ��ɫ��
    backgroundColorButton->setAutoFillBackground(true);
    // ���°�ť��ɫ
    QPalette pal = backgroundColorButton->palette();
    pal.setColor(QPalette::Button, m_backgroundColor);
    backgroundColorButton->setPalette(pal);

    connect(backgroundColorButton, &QPushButton::clicked, this, &SideSettingView::onBackgroundColorButtonClicked);
    viewSettingsLayout->addRow(u8"������ɫ:", backgroundColorButton);

    // ��ʾ����
    showGridCheckBox = new QCheckBox(u8"��ʾ��������", this);
    showGridCheckBox->setChecked(true);
    connect(showGridCheckBox, &QCheckBox::toggled, this, &SideSettingView::gridVisibleChanged);
    viewSettingsLayout->addRow(showGridCheckBox);

    // ����ƶ��ٶ�
    cameraMoveSpeedSlider = new QSlider(Qt::Horizontal, this);
    cameraMoveSpeedSlider->setRange(1, 100);
    cameraMoveSpeedSlider->setValue(50);
    connect(cameraMoveSpeedSlider, &QSlider::valueChanged, this, &SideSettingView::cameraMoveSpeedChanged);
    viewSettingsLayout->addRow(u8"����ƶ��ٶ�:", cameraMoveSpeedSlider);

    // �����ת�ٶ�
    cameraRotateSpeedSlider = new QSlider(Qt::Horizontal, this);
    cameraRotateSpeedSlider->setRange(1, 100);
    cameraRotateSpeedSlider->setValue(50);
    connect(cameraRotateSpeedSlider, &QSlider::valueChanged, this, &SideSettingView::cameraRotateSpeedChanged);
    viewSettingsLayout->addRow(u8"�����ת�ٶ�:", cameraRotateSpeedSlider);

    // ��Ұ (FOV)
    fovSpinBox = new QDoubleSpinBox(this);
    fovSpinBox->setRange(20.0, 120.0);
    fovSpinBox->setValue(60.0);
    fovSpinBox->setSuffix(" ��");
    connect(fovSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SideSettingView::fieldOfViewChanged);
    viewSettingsLayout->addRow(u8"��Ұ (FOV):", fovSpinBox);

    scrollAreaLayout->addWidget(viewSettingsGroup);

    // --- 2. ��������Ⱦ Group ---
    performanceGroup = new QGroupBox(u8"��������Ⱦ", this);
    performanceLayout = new QFormLayout(performanceGroup);

    // V-Sync
    vsyncCheckBox = new QCheckBox(u8"���ô�ֱͬ��", this);
    connect(vsyncCheckBox, &QCheckBox::toggled, this, &SideSettingView::vsyncChanged);
    performanceLayout->addRow(vsyncCheckBox);

    // ��������
    textureQualityComboBox = new QComboBox(this);
    textureQualityComboBox->addItems({ u8"��", u8"��", u8"��" });
    textureQualityComboBox->setCurrentIndex(2); // Ĭ�ϸ�
    connect(textureQualityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::textureQualityChanged);
    performanceLayout->addRow(u8"��������:", textureQualityComboBox);

    // �����
    antiAliasingComboBox = new QComboBox(this);
    antiAliasingComboBox->addItems({ u8"��", "FXAA", "MSAA x4" });
    antiAliasingComboBox->setCurrentIndex(1); // Ĭ�� FXAA
    connect(antiAliasingComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::antiAliasingChanged);
    performanceLayout->addRow(u8"�����:", antiAliasingComboBox);

    scrollAreaLayout->addWidget(performanceGroup);

    // --- 3. Ӧ������ Group ---
    appSettingsGroup = new QGroupBox(u8"Ӧ������", this);
    appSettingsLayout = new QFormLayout(appSettingsGroup);

    // UI ����
    themeComboBox = new QComboBox(this);
    themeComboBox->addItems({ u8"��ɫ����", u8"��ɫ����" });
    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SideSettingView::themeChanged);
    appSettingsLayout->addRow(u8"UI ����:", themeComboBox);

    // ������ť
    resetSettingsButton = new QPushButton(u8"�ָ�Ĭ������", this);
    aboutButton = new QPushButton(u8"����", this);
    connect(aboutButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, u8"����", u8"ģ�Ͳ鿴������� zrender_v1.0\n�� ZQTEngine ����");
        });

    // ����ť����һ��ˮƽ�����У�������ʾ
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetSettingsButton);
    buttonLayout->addWidget(aboutButton);
    appSettingsLayout->addRow(buttonLayout);

    scrollAreaLayout->addWidget(appSettingsGroup);

    // ���һ�����ɣ������������Ƶ�����
    scrollAreaLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // ��ɲ���
    scrollArea->setWidget(scrollAreaWidgetContents);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

// ������ɫ��ť�����ʱ�Ĳۺ���
void SideSettingView::onBackgroundColorButtonClicked() {
    QColor newColor = QColorDialog::getColor(m_backgroundColor, this, u8"ѡ�񱳾���ɫ");
    if (newColor.isValid()) {
        m_backgroundColor = newColor;

        // ���°�ť����ʾ��ɫ
        QPalette pal = backgroundColorButton->palette();
        pal.setColor(QPalette::Button, m_backgroundColor);
        backgroundColorButton->setPalette(pal);

        // �����źţ�֪ͨ�ⲿ����ɫ�Ѹı�
        emit backgroundColorChanged(m_backgroundColor);
    }
}