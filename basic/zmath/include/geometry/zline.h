#pragma once
#include"zmath.h"

#include"zvertex.h"

ZMATH_NS_BEGIN

class ZMATH_API ZLine {
private:
    ZVertex* start;
    ZVertex* end;

public:
    ZLine(ZVertex* s, ZVertex* e) : start(s), end(e) {}

    // ���������߶εĽ���
    bool intersects(const ZLine& other, ZVertex*& intersection) const {
        // ����ʹ��������˺��߶β������������㽻��
        // ���ڼ����Ϊ���ӣ�������ṩ��ܣ�����ʵ����Ҫ����ʵ�������д
        // ...
        return false; // ����н��㷵��true�����򷵻�false
    }
};
ZMATH_NS_END
