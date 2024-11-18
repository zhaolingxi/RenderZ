#pragma once
#include"zmath.h"

#include <cmath>
#include<string>
#include<vector>

#ifdef MATHEXPORT
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#else 
class gsl_vector;
#endif


ZMATH_NS_BEGIN
class ZMATH_API ZVertex
{
private:
    gsl_vector* pos_;
    gsl_vector* normal_;
    bool isInitialized_{false};
#define VX_PRESISION 1e-6 
public:
    ZVertex();
    ZVertex(double inX, double inY, double inZ);
    ZVertex(const std::string& input, char type);
    ~ZVertex();

    bool operator==(const ZVertex& other) const;
    static bool gsl_vector_equal(const gsl_vector* v1, const gsl_vector* v2);

    void parseCoordinates(const std::string& input);
    bool isInit();
    bool isInit() const;
    bool hasNormal();
    void setNormal(double x, double y, double z);
    void setNormal(gsl_vector* vec);

    std::vector<double> getPosStd();
    gsl_vector* getPos();
    gsl_vector* getPos() const;

    std::vector<double> getNormalStd();
    gsl_vector* getNormal();

    double X() const;
    double Y() const;
    double Z() const;
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    double findAngle( const ZVertex& v2);
};

ZMATH_NS_END