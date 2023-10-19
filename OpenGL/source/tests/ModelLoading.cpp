#include "ModelLoading.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {


	ModelLoading::ModelLoading()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,0,-6))
	{
		

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);


		
		m_Model = std::make_unique<OBJLoader>("resources/models/obj/testMonkey/testMonkey.obj");
		//m_Model = std::make_unique<OBJLoader>("resources/models/obj/testCube/testCube.obj");
		//m_Model = std::make_unique<OBJLoader>("resources/models/obj/testCivic/testCivic.obj");

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Model->m_Vertices.data(), m_Model->m_Vertices.size() * sizeof(float) * 8);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Model->m_Indices.data(), m_Model->m_Indices.size());

		m_Shader = std::make_unique<Shader>("resources/shaders/Model.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);


		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
	}

	ModelLoading::~ModelLoading()
	{
	}

	void ModelLoading::OnUpdate(float deltaTime)
	{
	}

	void ModelLoading::OnRender()
	{
		//window resizing
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		if (width != m_Width || height != m_Height) {
			m_Width = width;
			m_Height = height;
			m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
			GLCall(glViewport(0, 0, m_Width, m_Height));
		}


		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		


		Renderer renderer;

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		m_ModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMat = glm::rotate(m_ModelMat, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMat = glm::translate(m_ModelMat, m_ModelTranslation);
		glm::mat4 mvp = m_Proj * m_View * m_ModelMat;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);


	}

	void ModelLoading::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslation.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::ColorEdit4("Color B", m_Color);

	}
}