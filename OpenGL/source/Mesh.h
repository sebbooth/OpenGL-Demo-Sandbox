#pragma once


#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

class Mesh
{
public:
	VertexBuffer m_VBO;
	VertexArray m_VAO;
	IndexBuffer m_IBO;
	Texture m_Texture;
	Shader m_Shader;


	Mesh(std::string& filePath);
	~Mesh();


};

