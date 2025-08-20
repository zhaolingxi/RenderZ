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
            while (getline(stream, currLine)) {
                if (currLine.rfind("v ", 0) == 0) { // �ж��Ƿ��� "v " ��ͷ
                    ZVertex newV = this->parseVertex(currLine);
                    vertices_.push_back(newV);
                }
                else if (currLine.rfind("vn ", 0) == 0) { // �ж��Ƿ��� "vn " ��ͷ
                    ZVertex newN = this->parseVertex(currLine); // ���ߺͶ����ʽ��ͬ
                    normals_.push_back(newN);
                }
                else if (currLine.rfind("f ", 0) == 0) { // �ж��Ƿ��� "f " ��ͷ
                    ZFace newF = this->parseFace(currLine);
                    faces_.push_back(newF);
                }
            }
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



const std::vector<zmath::ZVertex>& ZObjModelLoader::getVertices() const {
    return vertices_;
}

const std::vector<zmath::ZFace>& ZObjModelLoader::getFacets() const {
    return faces_;
}

const std::vector<zmath::ZVertex>& ZObjModelLoader::getNormals() const {
    return normals_;
}


ZVertex ZObjModelLoader::parseVertex(std::string line)
{
    std::vector<double> coords = coordinateScanner(line);
    ZVertex vex(coords.at(0), coords.at(1), coords.at(2));
    return vex;
}

ZFace ZObjModelLoader::parseFace(std::string line) {
    // �滻���е�'/'Ϊ�ո���������ͳһ���� f v v v �� f v/vt/vn v/vt/vn v/vt/vn �ȸ�ʽ
    for (char& c : line) {
        if (c == '/') {
            c = ' ';
        }
    }

    std::stringstream ss(line);
    std::string keyword;
    ss >> keyword; // ���� 'f'

    ZFace face;
    int v_idx, vt_idx, vn_idx;

    // ѭ����ȡ��Ƭ�����ж���
    while (ss >> v_idx) {
        face.addVertexIndex(v_idx); // ����ZFace�������������Ӷ�������

        // ���Զ�ȡ����ͷ���������������Ǵ��ڵĻ�
        if (ss.peek() == ' ') {
            ss.ignore(); // �����ո�
            if (ss.peek() != ' ') { // ����Ƿ���������������
                ss >> vt_idx;
                face.addTexCoordIndex(vt_idx); // ����ZFace���������
            }
            if (ss.peek() == ' ') {
                ss.ignore();
                ss >> vn_idx;
                face.addNormalIndex(vn_idx); // ����ZFace���������
            }
        }
    }
    return face;
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


