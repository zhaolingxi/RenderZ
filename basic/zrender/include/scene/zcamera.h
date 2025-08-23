#pragma once
#include"zrender.h"

// 包含 Eigen 的核心头文件
#include <Eigen/Dense>
#include <Eigen/Geometry>

// 如果定义了这个宏，我们将包含Qt头文件并提供转换函数
#ifdef ZCAMERA_ENABLE_QT_INTEROP
#include <QVector3D>
#include <QMatrix4x4>
#endif
ZRENDER_NS_BEGIN

class ZRENDER_API ZCamera
{
public:
    ZCamera();

    // --- 核心功能：获取变换矩阵 ---
    Eigen::Matrix4f getViewMatrix() const;
    Eigen::Matrix4f getProjectionMatrix(float width, float height) const;

    // --- 用户交互 ---
    void rotate(float deltaX, float deltaY);
    void pan(float deltaX, float deltaY);
    void zoom(float delta);

    // --- 重置与状态 ---
    void reset();
    Eigen::Vector3f getPosition() const { return m_position; }
    Eigen::Vector3f getTarget() const { return m_target; }

    // --- Qt 互操作性接口 (仅在定义了宏时可用) ---
#ifdef ZCAMERA_ENABLE_QT_INTEROP
    QVector3D getPositionQt() const;
    QMatrix4x4 getViewMatrixQt() const;
#endif

private:
    // --- 相机状态 (使用 Eigen 类型) ---
    Eigen::Vector3f m_position; // 相机在世界坐标系中的位置
    Eigen::Vector3f m_target;   // 相机观察的目标点
    Eigen::Vector3f m_up;       // 相机的“上”方向

    // --- 投影参数 ---
    float m_fov;          // 视野角度 (Field of View)
    float m_nearPlane;    // 近裁剪面
    float m_farPlane;     // 远裁剪面

    // --- 交互灵敏度 ---
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
};
ZRENDER_NS_END

