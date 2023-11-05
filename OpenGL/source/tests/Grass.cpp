#include "Grass.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {


	Grass::Grass()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,2,-45)),
		m_ModelTranslation(glm::vec3(0,-4,-10))
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		m_Model = std::make_unique<OBJLoader>("resources/models/obj/grassBlade/grassBlade.obj");
		m_Texture = std::make_unique<Texture>("resources/models/obj/goblin/Face_diffuse.jpeg");

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

		m_RotationX = 8.0f;
		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);
	}

	Grass::~Grass()
	{
	}

	void Grass::OnUpdate(float deltaTime)
	{
		m_Time += deltaTime;
	}

	void Grass::OnRender()
	{
		Renderer renderer;

		renderer.updateProj(m_Window, m_Proj, m_Width, m_Height, 45.0f, 0.1f, 100000.0f);
		renderer.Clear();

		m_Texture->Bind();

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		m_ModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMat = glm::rotate(m_ModelMat, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMat = glm::translate(m_ModelMat, m_ModelTranslation);
		glm::mat4 mvp = m_Proj * m_View * m_ModelMat;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniformMat4f("u_Mod", m_ModelMat);
		m_Shader->SetUniformVec3("u_ViewPos", m_ViewTranslation);

		m_Shader->SetUniform1f("u_Time", m_Time);
		m_Shader->SetUniform1f("u_FogDist", m_FogDist);

		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);

		renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, 100000);

	}

	void Grass::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslation.x, -10.0f, 10.0f);

		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::SliderFloat("Fog", &m_FogDist, 0.0f, 2000.0f);

		ImGui::ColorEdit4("Obj Color", m_ModelCol);

	}
}