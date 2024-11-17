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

    // 计算两条线段的交点
    bool intersects(const ZLine& other, ZVertex*& intersection) const {
        // 这里使用向量叉乘和线段参数方程来计算交点
        // 由于计算较为复杂，这里仅提供框架，具体实现需要根据实际情况编写
        // ...
        return false; // 如果有交点返回true，否则返回false
    }
};
ZMATH_NS_END
