#pragma once
#include"zmath.h"

#include"zvertex.h"

// 平面 ZPlane
ZMATH_NS_BEGIN

class ZMATH_API ZPlane {
private:
    gsl_vector* normal;
    double d;

public:
    ZPlane(gsl_vector* n, double dist) : normal(n), d(dist) {}

    // 计算法向量
    gsl_vector* normalVector() const {
        return normal;
    }

    // 计算与其他几何体的相交
    // ...
};
ZMATH_NS_END