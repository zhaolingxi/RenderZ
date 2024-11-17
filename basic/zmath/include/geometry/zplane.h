#pragma once
#include"zmath.h"

#include"zvertex.h"

// ƽ�� ZPlane
ZMATH_NS_BEGIN

class ZMATH_API ZPlane {
private:
    gsl_vector* normal;
    double d;

public:
    ZPlane(gsl_vector* n, double dist) : normal(n), d(dist) {}

    // ���㷨����
    gsl_vector* normalVector() const {
        return normal;
    }

    // ������������������ཻ
    // ...
};
ZMATH_NS_END