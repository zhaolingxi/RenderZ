#include "zvertex.h"

ZMATH_NS_BEGIN

ZVertex::ZVertex()
{
	gsl_vector_set_all(pos_, 0);
}

ZVertex::ZVertex(double inX, double inY, double inZ)
{
    pos_ = gsl_vector_alloc(3);
    gsl_vector_set(pos_, 0, inX);
    gsl_vector_set(pos_, 1, inY);
    gsl_vector_set(pos_, 2, inZ);
    isInitialized_ = true;
}

ZVertex::ZVertex(const std::string& input, char type)
{
    if (type == 'v' || type == 'n') {
        parseCoordinates(input);
        isInitialized_ = true;
    }
}

ZVertex::~ZVertex()
{
    gsl_vector_free(pos_);
}

bool ZVertex::operator==(const ZVertex& other) const {
    if (!isInitialized_ || !other.isInitialized_) {
        return false; // 如果任一对象未初始化，则它们不相等
    }
    // 比较pos_
    if (gsl_vector_equal(pos_, other.pos_)) {
        // 比较normal_
        return gsl_vector_equal(normal_, other.normal_);
    }
    return false;
}

bool ZVertex::gsl_vector_equal(const gsl_vector* v1, const gsl_vector* v2) {
    if (v1 == v2) return true; // 相同对象
    if (!v1 || !v2 || v1->size != v2->size) return false; // 检查有效性和大小

    for (std::size_t i = 0; i < v1->size; ++i) {
        if (gsl_vector_get(v1, i) != gsl_vector_get(v2, i)) {
            return false;
        }
    }
    return true;
}

void ZVertex::parseCoordinates(const std::string& input)
{
    double x, y, z;
    sscanf(input.c_str(), "%*s %f %f %f", &x, &y, &z);
    gsl_vector_set(pos_, 0, x);
    gsl_vector_set(pos_, 1, y);
    gsl_vector_set(pos_, 2, z);
}

bool ZVertex::isInit() {
    return isInitialized_;
}

bool ZVertex::isInit() const {
    return isInitialized_;
}

bool ZVertex::hasNormal() {
    return !(int)(normal_->size);
}

std::vector<double> ZVertex::getPosStd()
{
    std::vector<double> pos(pos_->size);
    for (int i = 0; i < pos_->size; i++) {
        pos[i] = pos_->data[i];
    }
    return pos;
}
gsl_vector* ZVertex::getPos()
{
    return pos_;
}

gsl_vector* ZVertex::getPos() const
{
    return pos_;
}

std::vector<double> ZVertex::getNormalStd() {
    std::vector<double> normal(normal_->size);
    for (int i = 0; i < normal_->size; i++) {
        normal[i] = normal_->data[i];
    }
    return normal;
}

gsl_vector* ZVertex::getNormal() {
    return normal_;
}

void ZVertex::setNormal(double x, double y, double z)
{
    gsl_vector_set(normal_, 0, x);
    gsl_vector_set(normal_, 1, y);
    gsl_vector_set(normal_, 2, z);
}

void ZVertex::setNormal(gsl_vector* vec)
{
    normal_ = vec;
}

double ZVertex::X() const {
    return gsl_vector_get(pos_, 0);
}

double ZVertex::Y() const {
    return gsl_vector_get(pos_, 1);
}

double ZVertex::Z() const {
    return gsl_vector_get(pos_, 2);
}

void ZVertex::setX(double x) {
    gsl_vector_set(pos_, 0, x);
}

void ZVertex::setY(double y) {
    gsl_vector_set(pos_, 1, y);
}

void ZVertex::setZ(double z) {
    gsl_vector_set(pos_, 2, z);
}



 
double ZVertex::findAngle(const ZVertex& v2)
{
    if (!v2.isInit() || !isInit()) {
        return 0.0;
    }
    double dot, alsqr, blsqr, theta;
    gsl_blas_ddot(pos_, v2.getPos(), &dot);

    double norm_v1 = gsl_blas_dnrm2(pos_);
    double norm_v2 = gsl_blas_dnrm2(v2.getPos());

    // 计算角度的余弦值
    double cos_theta = dot / (norm_v1 * norm_v2);

    // 计算角度（以弧度为单位）
    double angle_rad = acos(cos_theta);

    double angle_deg = angle_rad * (180.0 / M_PI);

    return angle_deg; // 返回角度（以度为单位）
}

ZMATH_NS_END