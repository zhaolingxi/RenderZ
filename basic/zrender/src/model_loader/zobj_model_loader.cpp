#include"zobj_model_loader.h"
using namespace zmath;

ZRENDER_NS_BEGIN
ZObjModelLoader::ZObjModelLoader()
{
}

ZObjModelLoader::ZObjModelLoader(std::string objPath)
{
    objfilePath_ = objPath;
    std::ifstream stream(objPath.c_str());
    if (stream.is_open())
    {
        std::string currLine;
        while (!stream.eof())
        {
            /* Get next line of file */
            getline(stream, currLine);
            /* Determine whether it is a vertex, normal, or face line */
            //if (currLine[0] == 'v' && currLine[1] == ' ')
            //{
            //    ZVertex newV = this->parseVertex(currLine);
            //    vertices.push_back(newV);
            //    this->checkMax(newV);
            //    this->checkMin(newV);

            //}
            //else if (currLine[0] == 'v' && currLine[1] == 'n')
            //{
            //    Vertex n(currLine, currLine[1]);
            //    normals.push_back(n);
            //}

            //else if (currLine[0] == 'f' && currLine[1] == ' ')
            //{
            //    vector<Vertex> v = this->parseFace(currLine);
            //    face f = face(v.at(0), v.at(1), v.at(2));
            //    facets.push_back(f);
            //}
        }
        stream.close();
    }
}

ZObjModelLoader::~ZObjModelLoader()
{
}

std::string ZObjModelLoader::getObjPath(){
    return objfilePath_;
}

std::vector<zmath::ZVertex> ZObjModelLoader::getVertices() {
    return vertices_;
}
std::vector<zmath::ZFace> ZObjModelLoader::getFacets() {
    return faces_;
}

ZVertex ZObjModelLoader::parseVertex(std::string line)
{
    std::vector<double> coords = coordinateScanner(line);
    ZVertex vex(coords.at(0), coords.at(1), coords.at(2));
    return vex;
}

std::vector<zmath::ZVertex> ZObjModelLoader::parseFace(std::string line) {
    std::vector<ZVertex> toReturn;
    /* Parses differently, depending on if there are slashes in line */
    if (line.find("/") != std::string::npos) {
        /* remove the "f" in front of the line */
        line.erase(0, 1);
        std::string delimiter = "/";
        size_t pos = 0;
        std::string token;
        std::string token2;
        int index;
        int prevIndex = 0;
        while ((pos = line.find(delimiter) != std::string::npos))
        {
            /* Token is index of vertex in the face */
            token = line.substr(0, line.find(delimiter));
            std::istringstream(token) >> index; /* Only succeeds if token is an integer */
            if (index != prevIndex) {
                /* .obj file is indexed at 1, C++ vector indexed at 0 */
                toReturn.push_back(vertices_.at(index - 1));
                prevIndex = index;
            }
            /* Tossing out token2 until further notice */
            token2 = line.substr(line.find(delimiter), line.find(' '));
            /* Erases everything up until next index */
            line.erase(0, token.length() + token2.length());
        }
    }
    else {
        /* If there aren't any slashes, parses like coordinates */
        std::vector<double> indices = coordinateScanner(line);
        toReturn.push_back(vertices_.at(indices.at(0) - 1));
        toReturn.push_back(vertices_.at(indices.at(1) - 1));
        toReturn.push_back(vertices_.at(indices.at(2) - 1));
    }
    return toReturn;
}

std::vector<double>ZObjModelLoader::coordinateScanner(std::string line)
{
    std::vector<double> toReturn;
    float vertexArr[3];
    sscanf(line.c_str(), "%*s %f %f %f", &vertexArr[0], &vertexArr[1], &vertexArr[2]);
    toReturn.push_back(vertexArr[0]);
    toReturn.push_back(vertexArr[1]);
    toReturn.push_back(vertexArr[2]);
    return toReturn;
}

ZRENDER_NS_END


