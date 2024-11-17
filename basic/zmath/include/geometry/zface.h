#pragma once
#include"zmath.h"

#include"zvertex.h"
// ������ ZFace
ZMATH_NS_BEGIN

class ZMATH_API ZFace {
private:
    ZVertex* v1_, * v2_, * v3_;

public:
    ZFace(ZVertex* v1, ZVertex* v2, ZVertex* v3) : v1_(v1), v2_(v2), v3_(v3) {}

    bool operator==(const ZFace& other) const;
    ZVertex* getVertex(unsigned number);

    // ���㷨����
    ZVertex normalVector() const;

    // ������������������ཻ
    // ...
};
ZMATH_NS_END