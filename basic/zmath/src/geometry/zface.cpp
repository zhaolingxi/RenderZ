#include"zface.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
ZMATH_NS_BEGIN
//bool ZFace::operator==(const ZFace& other) const
//{
//    if (v1_== other.v1_ &&
//        v2_ == other.v2_ &&
//        v3_ == other.v3_) {
//        return true;
//    }
//    return false;
//}

//ZVertex* ZFace::getVertex(unsigned number)
//{
//    if (number == 1) {
//        return v1_;
//    }
//    else if (number == 2) {
//        return v2_;
//    }
//    else if (number == 3) {
//        return v3_;
//    }
//    return nullptr;
//}
//
//ZVertex ZFace::normalVector() const {
//    gsl_vector* v1v2 = gsl_vector_alloc(3);
//    gsl_vector* v1v3 = gsl_vector_alloc(3);
//    gsl_vector* n = gsl_vector_alloc(3);
//
//    gsl_vector_memcpy(v1v2, v2_->getPos());
//    gsl_vector_sub(v1v2, v1_->getPos());
//    gsl_vector_memcpy(v1v3, v3_->getPos());
//    gsl_vector_sub(v1v3, v1_->getPos());
//
//    // 计算叉积
//    gsl_vector_set(n, 0, gsl_vector_get(v1v2, 1) * gsl_vector_get(v1v3, 2) - gsl_vector_get(v1v2, 2) * gsl_vector_get(v1v3, 1));
//    gsl_vector_set(n, 1, gsl_vector_get(v1v2, 2) * gsl_vector_get(v1v3, 0) - gsl_vector_get(v1v2, 0) * gsl_vector_get(v1v3, 2));
//    gsl_vector_set(n, 2, gsl_vector_get(v1v2, 0) * gsl_vector_get(v1v3, 1) - gsl_vector_get(v1v2, 1) * gsl_vector_get(v1v3, 0));
//
//    gsl_vector_memcpy(v1v2, n); 
//
//    gsl_vector_free(v1v3);
//
//    // 取单位向量
//    double norm = gsl_blas_dnrm2(n);
//    if (norm != 0) {
//        gsl_vector_scale(n, 1.0 / norm);
//    }
//    else {
//        // 如果向量是零向量，需要处理这种情况
//        gsl_vector_set_zero(n);
//    }
//
//    ZVertex vertex(n->data[0], n->data[1], n->data[2]);
//    return vertex;
//}
ZMATH_NS_END