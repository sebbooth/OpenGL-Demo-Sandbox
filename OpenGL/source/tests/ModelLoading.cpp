#include "ModelLoading.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	ModelLoading::ModelLoading()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,0,-2))
	{
		

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);

		m_Shader = std::make_unique<Shader>("resources/shaders/Model.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		m_Model = std::make_unique<GLTFLoader>("resources/models/cube.gltf");

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
		

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);

		m_Model->Draw(*m_Shader, m_Proj, m_View);
		
	}

	void ModelLoading::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -10.0f, 10.0f);
	}
}