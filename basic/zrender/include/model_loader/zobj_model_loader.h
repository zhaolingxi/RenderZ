#pragma once
#include"zrender.h"
#include <iostream>
#include<fstream>
#include <sstream>
#include<vector>
#include<string>
#include<iterator>
#include "geometry/zvertex.h"
#include "geometry/zface.h"
ZRENDER_NS_BEGIN
class ZRENDER_API ZObjModelLoader
{
private:
    std::string objfilePath_;
    std::vector<zmath::ZVertex> vertices_;
    std::vector<zmath::ZFace> faces_;
    std::vector<zmath::ZVertex> normals_;
    zmath::ZVertex maxCoords_;
    zmath::ZVertex minCoords_;
    zmath::ZVertex center_;
    double radius_;
public:
    ZObjModelLoader();
    ZObjModelLoader(std::string objPath);
    ~ZObjModelLoader();

    std::string getObjPath();
    const std::vector<zmath::ZVertex>& getVertices() const;

    const std::vector<zmath::ZFace>& getFacets() const;

    const std::vector<zmath::ZVertex>& getNormals() const;

    zmath::ZVertex parseVertex(std::string line);
    ZFace parseFace(std::string line);

    std::vector<double> coordinateScanner(std::string line);
 };
ZRENDER_NS_END