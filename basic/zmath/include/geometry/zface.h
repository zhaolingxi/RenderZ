#pragma once
#include "zmath.h"
#include "geometry/zvertex.h"
#include <vector>

ZMATH_NS_BEGIN

class ZMATH_API ZFace
{
public:
    ZFace() = default;
    ~ZFace() = default;

    // ����������Ӷ��㡢�������ꡢ���ߵ�����
    void addVertexIndex(int index) { vertex_indices_.push_back(index); }
    void addTexCoordIndex(int index) { texcoord_indices_.push_back(index); }
    void addNormalIndex(int index) { normal_indices_.push_back(index); }

    // ����������ȡ�����б�� const ����
    const std::vector<int>& getVertexIndices() const { return vertex_indices_; }
    const std::vector<int>& getTexCoordIndices() const { return texcoord_indices_; }
    const std::vector<int>& getNormalIndices() const { return normal_indices_; }

private:
    std::vector<int> vertex_indices_;
    std::vector<int> texcoord_indices_;
    std::vector<int> normal_indices_;
};

ZMATH_NS_END