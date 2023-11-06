#include "HeightMapTerrain.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "TerrainGrid.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	HeightMapTerrain::HeightMapTerrain()
		: m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Proj(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,-300,-50)), 
		m_ModelTranslation(glm::vec3(0, 0, 0))
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
		m_Shader->SetUniform1f("u_GridW", (float)gridNum);
		m_Shader->SetUniform1f("u_GridL", (float)gridNum);
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
	}

	HeightMapTerrain::~HeightMapTerrain() 
	{
	}

	void HeightMapTerrain::OnUpdate(float deltaTime)
	{
	}

	void HeightMapTerrain::OnRender()
	{

		Renderer renderer;

		renderer.updateProj(m_Window, m_Proj, m_Width, m_Height, 45.0f, 0.1f, 10000.0f);
		renderer.Clear();

		m_Texture->Bind();

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		{
			m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
			m_Model = glm::rotate(m_Model, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
			m_Model = glm::translate(m_Model, m_ModelTranslation);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void HeightMapTerrain::OnImGuiRender()
	{

		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -300.0f, 300.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslation.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::ColorEdit4("Color B", m_Color);


	}
}