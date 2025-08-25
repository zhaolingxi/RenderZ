#pragma once

#include "zqtgui.h" // ��������������ĿAPI��

#include <QWidget>
#include <QColor>

// ǰ���������н��õ���Qt�࣬���Լ���ͷ�ļ���������
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

    // --- �ź��� ---
    // ��UI�ؼ������仯ʱ���ᷢ����Щ�ź�
    // ����3D��Ⱦ��������ģ��������ӵ���Щ�ź��Խ��ո���
signals:
    // ��ͼ�����ź�
    void backgroundColorChanged(const QColor& color);
    void gridVisibleChanged(bool visible);
    void cameraMoveSpeedChanged(int value);
    void cameraRotateSpeedChanged(int value);
    void fieldOfViewChanged(double value);

    // ��������Ⱦ�ź�
    void vsyncChanged(bool enabled);
    void textureQualityChanged(int index); // 0: Low, 1: Medium, 2: High
    void antiAliasingChanged(int index);   // 0: None, 1: FXAA, 2: MSAA x4

    // Ӧ�������ź�
    void themeChanged(int index); // 0: Dark, 1: Light

private slots:
    // ���ڴ����ض��߼���˽�вۺ���
    void onBackgroundColorButtonClicked();

private:
    // ��ʼ��UI�ĸ�������
    void setupUi();

    // ָ��UI�ؼ���ָ��
    // --- ���� ---
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* scrollAreaWidgetContents;
    QVBoxLayout* scrollAreaLayout;

    // --- ��ͼ���� Group ---
    QGroupBox* viewSettingsGroup;
    QFormLayout* viewSettingsLayout;
    QPushButton* backgroundColorButton;
    QCheckBox* showGridCheckBox;
    QSlider* cameraMoveSpeedSlider;
    QSlider* cameraRotateSpeedSlider;
    QDoubleSpinBox* fovSpinBox;

    // --- ��������Ⱦ Group ---
    QGroupBox* performanceGroup;
    QFormLayout* performanceLayout;
    QCheckBox* vsyncCheckBox;
    QComboBox* textureQualityComboBox;
    QComboBox* antiAliasingComboBox;

    // --- Ӧ������ Group ---
    QGroupBox* appSettingsGroup;
    QFormLayout* appSettingsLayout;
    QComboBox* themeComboBox;
    QPushButton* resetSettingsButton;
    QPushButton* aboutButton;

    // ��Ա����
    QColor m_backgroundColor;
};