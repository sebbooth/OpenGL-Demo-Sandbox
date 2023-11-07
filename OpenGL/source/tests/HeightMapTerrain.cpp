#include "HeightMapTerrain.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "TerrainGrid.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	HeightMapTerrain::HeightMapTerrain()
		: m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Proj(glm::mat4(1.0f))
	{
		
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);


		float gridSize = 1.0f;
		int gridNum = 300;
		TerrainGrid grid = TerrainGrid(gridNum, gridNum, gridSize);

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(grid.m_Vertices.data(), grid.m_Vertices.size() * sizeof(float) * 3);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(grid.m_Indices.data(), grid.m_Indices.size());

		m_Shader = std::make_unique<Shader>("resources/shaders/Terrain.glsl");

		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("resources/textures/heightmap2.png");

		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform1f("u_GridSize", gridSize);
		m_Shader->SetUniform1f("u_MaxHeight", m_MaxHeight);
		m_Shader->SetUniform1f("u_GridW", (float)gridNum);
		m_Shader->SetUniform1f("u_GridL", (float)gridNum);
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 10000.0f);
	}

	HeightMapTerrain::~HeightMapTerrain() 
	{
	}

	void HeightMapTerrain::OnUpdate(float deltaTime)
	{
		float moveSpeed = 0.05f;
		float rotateSpeed = 0.0008f;

		if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			glm::mat4 rotationMat(1);
			rotationMat = glm::rotate(rotationMat, rotateSpeed * deltaTime, m_CamUp);
			m_CamDir = glm::vec3(rotationMat * glm::vec4(m_CamDir, 1.0));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			glm::mat4 rotationMat(1);
			rotationMat = glm::rotate(rotationMat, -rotateSpeed * deltaTime, m_CamUp);
			m_CamDir = glm::vec3(rotationMat * glm::vec4(m_CamDir, 1.0));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			glm::mat4 rotationMat(1);
			glm::vec3 camLeft = glm::cross(m_CamUp, m_CamDir);
			rotationMat = glm::rotate(rotationMat, -rotateSpeed * deltaTime, camLeft);
			m_CamDir = glm::vec3(rotationMat * glm::vec4(m_CamDir, 1.0));
			m_CamUp = glm::vec3(rotationMat * glm::vec4(m_CamUp, 1.0));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			glm::mat4 rotationMat(1);
			glm::vec3 camLeft = glm::cross(m_CamUp, m_CamDir);
			rotationMat = glm::rotate(rotationMat, rotateSpeed * deltaTime, camLeft);
			m_CamDir = glm::vec3(rotationMat * glm::vec4(m_CamDir, 1.0));
			m_CamUp = glm::vec3(rotationMat * glm::vec4(m_CamUp, 1.0));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_CamPos += deltaTime * moveSpeed * m_CamDir;
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_CamPos -= deltaTime * moveSpeed * m_CamDir;
		if (glfwGetKey(m_Window, GLFW_KEY_R) == GLFW_PRESS)
			m_CamPos += deltaTime * moveSpeed * m_CamUp;
		if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS)
			m_CamPos -= deltaTime * moveSpeed * m_CamUp;

		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			glm::vec3 camLeft = glm::cross(m_CamUp, m_CamDir);
			m_CamPos += deltaTime * moveSpeed * camLeft;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			glm::vec3 camRight = glm::cross(m_CamDir, m_CamUp);
			m_CamPos += deltaTime * moveSpeed * camRight;
		}
		m_View = glm::lookAt(m_CamPos, m_CamPos + m_CamDir, m_CamUp);
	}

	void HeightMapTerrain::OnRender()
	{

		Renderer renderer;

		renderer.updateProj(m_Window, m_Proj, m_Width, m_Height, 45.0f, 0.1f, 10000.0f);
		renderer.setClearColor(0.1f, 0.1f, 0.3f, 1.0f);
		renderer.Clear();
		
		m_Texture->Bind();



		
		m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Model = glm::rotate(m_Model, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 mvp = m_Proj * m_View * m_Model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform1f("u_MaxHeight", m_MaxHeight);
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);	
	}

	void HeightMapTerrain::OnImGuiRender()
	{
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::SliderFloat("Terrain Height", &m_MaxHeight, -200.0f, 200.0f);
		ImGui::ColorEdit4("Color", m_Color);
	}
}