#pragma once
#include"zrender.h"

// ���� Eigen �ĺ���ͷ�ļ�
#include <Eigen/Dense>
#include <Eigen/Geometry>

// �������������꣬���ǽ�����Qtͷ�ļ����ṩת������
#ifdef ZCAMERA_ENABLE_QT_INTEROP
#include <QVector3D>
#include <QMatrix4x4>
#endif
ZRENDER_NS_BEGIN

class ZRENDER_API ZCamera
{
public:
    ZCamera();

    // --- ���Ĺ��ܣ���ȡ�任���� ---
    Eigen::Matrix4f getViewMatrix() const;
    Eigen::Matrix4f getProjectionMatrix(float width, float height) const;

    // --- �û����� ---
    void rotate(float deltaX, float deltaY);
    void pan(float deltaX, float deltaY);
    void zoom(float delta);

    // --- ������״̬ ---
    void reset();
    Eigen::Vector3f getPosition() const { return m_position; }
    Eigen::Vector3f getTarget() const { return m_target; }

    // --- Qt �������Խӿ� (���ڶ����˺�ʱ����) ---
#ifdef ZCAMERA_ENABLE_QT_INTEROP
    QVector3D getPositionQt() const;
    QMatrix4x4 getViewMatrixQt() const;
#endif

private:
    // --- ���״̬ (ʹ�� Eigen ����) ---
    Eigen::Vector3f m_position; // �������������ϵ�е�λ��
    Eigen::Vector3f m_target;   // ����۲��Ŀ���
    Eigen::Vector3f m_up;       // ����ġ��ϡ�����

    // --- ͶӰ���� ---
    float m_fov;          // ��Ұ�Ƕ� (Field of View)
    float m_nearPlane;    // ���ü���
    float m_farPlane;     // Զ�ü���

    // --- ���������� ---
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
};
ZRENDER_NS_END

