#pragma once


#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

class Mesh
{
public:
	std::vector <float> m_Vertices;
	std::vector <GLuint> m_Indices;
	std::vector <Texture> m_Textures;




	// Initializes the mesh
	Mesh(std::vector<float>& vertices, VertexBufferLayout layout, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 400.0f/300.0f, 0.1f, 100.0f),
		glm::mat4 model = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);


private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Texture> m_Texture;
};

