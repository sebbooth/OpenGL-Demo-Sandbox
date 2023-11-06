#include "OBJLoader.h"


OBJLoader::OBJLoader(const std::string& filePath)
{
    unsigned int indexNum = 0;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v") {
            float x, y, z;
            stream >> x >> y >> z;
            positions.push_back(glm::vec3(x, y, z));
        }
        else if (prefix == "vn") {
            float nx, ny, nz;
            stream >> nx >> ny >> nz;
            normals.push_back(glm::vec3(nx, ny, nz));
        }
        else if (prefix == "vt") {
            float u, v;
            stream >> u >> v;
            texCoords.push_back(glm::vec2(u, v));
        }
        else if (prefix == "f") {
            std::string v1, v2, v3, v4;
            int pIdx1, pIdx2, pIdx3, pIdx4, nIdx1, nIdx2, nIdx3, nIdx4, tIdx1, tIdx2, tIdx3, tIdx4;
            char slash;

            stream >> v1 >> v2 >> v3 >> v4;

            if (std::count(v1.begin(), v1.end(), '/') == 2 &&
                std::count(v2.begin(), v2.end(), '/') == 2 &&
                std::count(v3.begin(), v3.end(), '/') == 2 &&
                std::count(v4.begin(), v4.end(), '/') == 2)
            {
                
                std::istringstream v1Stream(v1), v2Stream(v2), v3Stream(v3), v4Stream(v4);

                v1Stream >> pIdx1 >> slash >> tIdx1 >> slash >> nIdx1;
                v2Stream >> pIdx2 >> slash >> tIdx2 >> slash >> nIdx2;
                v3Stream >> pIdx3 >> slash >> tIdx3 >> slash >> nIdx3;
                v4Stream >> pIdx4 >> slash >> tIdx4 >> slash >> nIdx4;

                OBJVertex vertex1 = {
                    positions[pIdx1 - 1].x, positions[pIdx1 - 1].y, positions[pIdx1 - 1].z,
                    normals[nIdx1 - 1].x, normals[nIdx1 - 1].y, normals[nIdx1 - 1].z,
                    texCoords[tIdx1 - 1].x, texCoords[tIdx1 - 1].y
                };

                OBJVertex vertex2 = {
                    positions[pIdx2 - 1].x, positions[pIdx2 - 1].y, positions[pIdx2 - 1].z,
                    normals[nIdx2 - 1].x, normals[nIdx2 - 1].y, normals[nIdx2 - 1].z,
                    texCoords[tIdx2 - 1].x, texCoords[tIdx2 - 1].y
                };

                OBJVertex vertex3 = {
                    positions[pIdx3 - 1].x, positions[pIdx3 - 1].y, positions[pIdx3 - 1].z,
                    normals[nIdx3 - 1].x, normals[nIdx3 - 1].y, normals[nIdx3 - 1].z,
                    texCoords[tIdx3 - 1].x, texCoords[tIdx3 - 1].y
                };

                OBJVertex vertex4 = {
                    positions[pIdx4 - 1].x, positions[pIdx4 - 1].y, positions[pIdx4 - 1].z,
                    normals[nIdx4 - 1].x, normals[nIdx4 - 1].y, normals[nIdx4 - 1].z,
                    texCoords[tIdx4 - 1].x, texCoords[tIdx4 - 1].y
                };

                // Triangulate the quad
                m_Vertices.push_back(vertex1);
                m_Vertices.push_back(vertex2);
                m_Vertices.push_back(vertex3);
                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);

                m_Vertices.push_back(vertex1);
                m_Vertices.push_back(vertex3);
                m_Vertices.push_back(vertex4);
                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);
            }
            else
            {
                std::istringstream v1Stream(v1), v2Stream(v2), v3Stream(v3);

                v1Stream >> pIdx1 >> slash >> tIdx1 >> slash >> nIdx1;
                v2Stream >> pIdx2 >> slash >> tIdx2 >> slash >> nIdx2;
                v3Stream >> pIdx3 >> slash >> tIdx3 >> slash >> nIdx3;

                OBJVertex vertex1 = {
                    positions[pIdx1 - 1].x, positions[pIdx1 - 1].y, positions[pIdx1 - 1].z,
                    normals[nIdx1 - 1].x, normals[nIdx1 - 1].y, normals[nIdx1 - 1].z,
                    texCoords[tIdx1 - 1].x, texCoords[tIdx1 - 1].y
                };

                OBJVertex vertex2 = {
                    positions[pIdx2 - 1].x, positions[pIdx2 - 1].y, positions[pIdx2 - 1].z,
                    normals[nIdx2 - 1].x, normals[nIdx2 - 1].y, normals[nIdx2 - 1].z,
                    texCoords[tIdx2 - 1].x, texCoords[tIdx2 - 1].y
                };

                OBJVertex vertex3 = {
                    positions[pIdx3 - 1].x, positions[pIdx3 - 1].y, positions[pIdx3 - 1].z,
                    normals[nIdx3 - 1].x, normals[nIdx3 - 1].y, normals[nIdx3 - 1].z,
                    texCoords[tIdx3 - 1].x, texCoords[tIdx3 - 1].y
                };

                m_Vertices.push_back(vertex1);
                m_Vertices.push_back(vertex2);
                m_Vertices.push_back(vertex3);

                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);
                m_Indices.push_back(indexNum++);
            }
            
        }
    }

    file.close();
}

void OBJLoader::Draw(Shader& shader, glm::mat4 proj, glm::mat4 view)
{

}

void OBJLoader::addTexture(const std::string& filePath, const char type)
{
}


