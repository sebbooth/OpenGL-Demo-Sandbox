#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct OBJVertex {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

class OBJLoader
{
public:
	OBJLoader(const std::string& filePath);


	void Draw
	(
		Shader& shader,
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 400.0f / 300.0f, 0.1f, 100.0f),
		glm::mat4 view = glm::mat4(1.0f)
	);

	void addTexture(const std::string& filePath, const char type);

private:
	std::string m_FilePath;

	std::vector<OBJVertex> m_Vertices;

	// Prevents textures from being loaded twice
	std::vector<std::string> m_LoadedTexNames;

	std::vector<Texture> m_Textures;

	std::vector<GLuint> m_Indices;
};

