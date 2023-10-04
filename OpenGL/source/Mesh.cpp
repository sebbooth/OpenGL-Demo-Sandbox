#include "Mesh.h"

Mesh::Mesh(std::vector<float>& vertices, VertexBufferLayout layout, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::m_Vertices = vertices;
	Mesh::m_Indices = indices;
	Mesh::m_Textures = textures;

	m_VAO = std::make_unique<VertexArray>();


	// Generates Vertex Buffer Object and links it to vertices
	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));

	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	// Generates Element Buffer Object and links it to indices
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

	// Unbind all to prevent accidentally modifying them
	m_VAO->UnBind();
	m_VertexBuffer->UnBind();
	m_IndexBuffer->UnBind();
}


void Mesh::Draw
(
	Shader& shader,
	glm::mat4 proj,
	glm::mat4 view,
	glm::mat4 model,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	Renderer renderer;
	m_VAO->Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		std::string num;
		std::string type = m_Textures[i].m_Type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		m_Textures[i].texUnit(shader, (type + num).c_str(), i);
		m_Textures[i].Bind(i);
	}
	
	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	//glm::mat4 mvp = proj * view * sca * rot * trans * model;
	glm::mat4 mvp = proj * view * model;


	shader.SetUniformMat4f("u_MVP", mvp);

	// Draw the actual mesh
	renderer.Draw(*m_VAO, *m_IndexBuffer, shader);
}