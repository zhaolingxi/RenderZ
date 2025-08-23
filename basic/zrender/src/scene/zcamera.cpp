#include "scene/zcamera.h"
#include <cmath> 
ZRENDER_NS_BEGIN

// --- ��������������ͼ��ѧר�þ��� ---

// ����һ�� "LookAt" ��ͼ����
static Eigen::Matrix4f createLookAtMatrix(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& worldUp)
{
    Eigen::Vector3f z_axis = (position - target).normalized();
    Eigen::Vector3f x_axis = worldUp.cross(z_axis).normalized();
    Eigen::Vector3f y_axis = z_axis.cross(x_axis);

    Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();
    viewMatrix.block<1, 3>(0, 0) = x_axis.transpose();
    viewMatrix.block<1, 3>(1, 0) = y_axis.transpose();
    viewMatrix.block<1, 3>(2, 0) = z_axis.transpose();

    // ���������������� float ֵ�����һ�� Vector3f �ٸ�ֵ
    viewMatrix.block<3, 1>(0, 3) = Eigen::Vector3f(-x_axis.dot(position), -y_axis.dot(position), -z_axis.dot(position));

    return viewMatrix;
}

// ����һ��͸��ͶӰ����
static Eigen::Matrix4f createPerspectiveMatrix(float fov_degrees, float aspectRatio, float zNear, float zFar)
{
    // ������������ EIGEN_PI ת��Ϊ float�����⾯��
    const float fov_rad = fov_degrees * (static_cast<float>(EIGEN_PI) / 180.0f);
    const float tanHalfFovy = std::tan(fov_rad * 0.5f);

    Eigen::Matrix4f perspectiveMatrix = Eigen::Matrix4f::Zero();

    perspectiveMatrix(0, 0) = 1.0f / (aspectRatio * tanHalfFovy);
    perspectiveMatrix(1, 1) = 1.0f / (tanHalfFovy);
    perspectiveMatrix(2, 2) = -(zFar + zNear) / (zFar - zNear);
    perspectiveMatrix(3, 2) = -1.0f;
    perspectiveMatrix(2, 3) = -(2.0f * zFar * zNear) / (zFar - zNear);

    return perspectiveMatrix;
}


// --- ZCamera ��ʵ�� ---

ZCamera::ZCamera()
{
    m_rotationSpeed = 0.4f;
    m_panSpeed = 0.01f;
    m_zoomSpeed = 0.1f;
    m_fov = 45.0f;
    m_nearPlane = 0.1f;
    m_farPlane = 100.0f;

    reset();
}

void ZCamera::reset()
{
    m_position = Eigen::Vector3f(0.0f, 0.0f, 5.0f);
    m_target = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    m_up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
}

Eigen::Matrix4f ZCamera::getViewMatrix() const
{
    return createLookAtMatrix(m_position, m_target, m_up);
}

Eigen::Matrix4f ZCamera::getProjectionMatrix(float width, float height) const
{
    float aspectRatio = (height > 0) ? (width / height) : 1.0f;
    return createPerspectiveMatrix(m_fov, aspectRatio, m_nearPlane, m_farPlane);
}

void ZCamera::rotate(float deltaX, float deltaY)
{
    Eigen::Vector3f viewVector = m_position - m_target;

    // �������������Ƕȼ�����ʹ�õ� PI ת��Ϊ float
    const float angleToRad = static_cast<float>(EIGEN_PI) / 180.0f;

    // Χ������Y����ת (ˮƽ)
    Eigen::Quaternionf rotY(Eigen::AngleAxisf(-deltaX * m_rotationSpeed * angleToRad, Eigen::Vector3f::UnitY()));

    // �������������
    Eigen::Vector3f rightVector = m_up.cross(viewVector.normalized()).normalized();

    // Χ�������������ת (��ֱ)
    Eigen::Quaternionf rotX(Eigen::AngleAxisf(-deltaY * m_rotationSpeed * angleToRad, rightVector));

    // �����ת
    Eigen::Quaternionf totalRotation = rotY * rotX;

    // Ӧ����ת
    m_position = m_target + totalRotation * viewVector;
    m_up = totalRotation * m_up;
}

void ZCamera::pan(float deltaX, float deltaY)
{
    Eigen::Vector3f forward = (m_target - m_position).normalized();
    Eigen::Vector3f right = forward.cross(m_up).normalized();
    Eigen::Vector3f up = right.cross(forward); // ���¼���һ���ϸ�ֱ���Ϸ���

    Eigen::Vector3f translation = (right * -deltaX * m_panSpeed) + (up * deltaY * m_panSpeed);

    m_position += translation;
    m_target += translation;
}

void ZCamera::zoom(float delta)
{
    Eigen::Vector3f viewVector = m_target - m_position;
    float distance = viewVector.norm();

    if (distance < 0.1f && delta > 0) {
        return;
    }

    m_position += viewVector.normalized() * delta * m_zoomSpeed;
}

// --- Qt �������Խӿ�ʵ�� ---
#ifdef ZCAMERA_ENABLE_QT_INTEROP
#include <QVector3D>
#include <QMatrix4x4>
QVector3D ZCamera::getPositionQt() const
{
    return QVector3D(m_position.x(), m_position.y(), m_position.z());
}

QMatrix4x4 ZCamera::getViewMatrixQt() const
{
    // Eigen ����Ĭ������������ QMatrix4x4 �� OpenGL ����
    // .data() ��������һ��ָ��ײ������ָ�룬����ֱ�����ڹ���
    Eigen::Matrix4f viewMatrix = getViewMatrix();
    // ʹ�� QMatrix4x4 �Ĺ��캯����������һ��ָ��16����������ָ��
    return QMatrix4x4(viewMatrix.data());
}
#endif

ZRENDER_NS_END
