// CREDIT:: 
// This code is a modified version of the GLTF model loader written by Victor Gordan
// in his openGL tutorial series on youtube.
// https://github.com/VictorGordan/opengl-tutorials

#pragma once
#include <string>
#include <fstream>


#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "json/json.hpp"
using json = nlohmann::json;



class GLTFLoader
{
public:
	GLTFLoader(const std::string& filePath);

	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));


	std::vector<float> getFloats(json accessor);

	std::vector<GLuint> getIndices(json accessor);

	std::vector<Texture> getTextures();

private:
	json m_JSON;
	std::vector<unsigned char> m_Data;
	std::string m_FilePath;

	// Prevents textures from being loaded twice
	std::vector<std::string> m_LoadedTexNames;
	std::vector<Texture> m_LoadedTex;


	// All the meshes and transformations
	std::vector<int> m_MeshesInd;
	std::vector<glm::vec3> m_TranslationsMeshes;
	std::vector<glm::quat> m_RotationsMeshes;
	std::vector<glm::vec3> m_ScalesMeshes;
	std::vector<glm::mat4> m_MatricesMeshes;

};

