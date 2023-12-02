#include "include/Grass.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "TerrainGrid.h"
#include <cmath>

namespace test {


	Grass::Grass()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f))
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_MULTISAMPLE));

		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		m_Model = std::make_unique<OBJLoader>("resources/models/obj/grassBlade/grassBlade.obj");
		m_Texture = std::make_unique<Texture>("resources/textures/heightmap1.png");

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Model->m_Vertices.data(), m_Model->m_Vertices.size() * sizeof(float) * 8);
		
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Model->m_Indices.data(), m_Model->m_Indices.size());

		m_Shader = std::make_unique<Shader>("resources/shaders/BasicGrass.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);
		m_Shader->SetUniform1f("u_GridNum", (float)m_GridNum);

		m_RotationX = 6.5f;
		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);

		float gridSize = 1.0f;
		TerrainGrid grid = TerrainGrid((int)sqrt((float)m_GridNum), (int)sqrt((float)m_GridNum), gridSize);

		m_PlaneVAO = std::make_unique<VertexArray>();
		m_PlaneVertexBuffer = std::make_unique<VertexBuffer>(grid.m_Vertices.data(), grid.m_Vertices.size() * sizeof(float) * 3);
		
		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3);
		m_PlaneVAO->AddBuffer(*m_PlaneVertexBuffer, planeLayout);

		m_PlaneIndexBuffer = std::make_unique<IndexBuffer>(grid.m_Indices.data(), grid.m_Indices.size());

		m_PlaneShader = std::make_unique<Shader>("resources/shaders/BasicGrassTerrain.glsl");
		m_PlaneShader->Bind();
		m_PlaneShader->SetUniform1i("u_Texture", 0);
		m_PlaneShader->SetUniform1f("u_GridSize", gridSize);
		m_PlaneShader->SetUniform1f("u_GridW", sqrt((float)m_GridNum));
		m_PlaneShader->SetUniform1f("u_GridL", sqrt((float)m_GridNum));
		m_PlaneShader->SetUniform4f("u_Color", m_PlaneCol[0], m_PlaneCol[1], m_PlaneCol[2], m_PlaneCol[3]);
	}

	Grass::~Grass()
	{
	}

	void Grass::OnUpdate(float deltaTime)
	{
		m_Time += deltaTime;

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

	void Grass::OnRender()
	{
		Renderer renderer;

		renderer.updateProj(m_Window, m_Proj, m_Width, m_Height, 45.0f, 0.1f, 100000.0f);
		renderer.Clear();

		m_Texture->Bind();

		m_ModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMat = glm::rotate(m_ModelMat, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 mvp = m_Proj * m_View * m_ModelMat;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform1f("u_Time", m_Time);
		m_Shader->SetUniform1f("u_FogDist", m_FogDist);
		m_Shader->SetUniform1f("u_GridNum", sqrt((float)m_GridNum));
		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);

		renderer.setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, m_GridNum);

		m_PlaneShader->Bind();
		m_PlaneShader->SetUniformMat4f("u_MVP", mvp);
		m_PlaneShader->SetUniform1f("u_FogDist", m_FogDist);
		m_PlaneShader->SetUniform4f("u_Color", m_PlaneCol[0], m_PlaneCol[1], m_PlaneCol[2], m_PlaneCol[3]);
		
		renderer.Draw(*m_PlaneVAO, *m_PlaneIndexBuffer, *m_PlaneShader);
	}

	void Grass::OnImGuiRender()
	{
		ImGui::SliderInt("Num Instances", &m_GridNum, 0.0f, 1000000.0f);
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::SliderFloat("Fog", &m_FogDist, 0.0f, 2000.0f);

		ImGui::ColorEdit4("Grass Color", m_ModelCol);
		ImGui::ColorEdit4("Terrain Color", m_PlaneCol);
	}
}