#include "ModelInstancing.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {


	ModelInstancing::ModelInstancing()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,0,-44))
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		m_Model = std::make_unique<OBJLoader>("resources/models/obj/goblin/goblin.obj");
		m_Texture = std::make_unique<Texture>("resources/models/obj/goblin/Face_diffuse.jpeg");

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Model->m_Vertices.data(), m_Model->m_Vertices.size() * sizeof(float) * 8);
		
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Model->m_Indices.data(), m_Model->m_Indices.size());

		m_Shader = std::make_unique<Shader>("resources/shaders/BasicModelInstanceGrid.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		m_RotationY = 180.0f;
		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);
	}

	ModelInstancing::~ModelInstancing()
	{
	}

	void ModelInstancing::OnUpdate(float deltaTime)
	{
	}

	void ModelInstancing::OnRender()
	{
		//window resizing
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		if (width != m_Width || height != m_Height) {
			m_Width = width;
			m_Height = height;
			m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);
			GLCall(glViewport(0, 0, m_Width, m_Height));
		}


		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		


		Renderer renderer;
		m_Texture->Bind();

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		m_ModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMat = glm::rotate(m_ModelMat, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMat = glm::translate(m_ModelMat, m_ModelTranslation);
		glm::mat4 mvp = m_Proj * m_View * m_ModelMat;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniformMat4f("u_Mod", m_ModelMat);


		m_Shader->SetUniform3f("u_LightColor", m_LightColor[0], m_LightColor[1], m_LightColor[2]);
		m_Shader->SetUniform1f("u_AmbientIntensity", m_AmbientIntensity);

		m_Shader->SetUniformVec3("u_ViewPos", m_ViewTranslation);
		m_Shader->SetUniformVec3("u_LightPos", m_LightPos);

		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);


		m_Shader->SetUniform1f("u_SpecularIntensity", m_SpecularIntensity);

		renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, 1000);


	}

	void ModelInstancing::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslation.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);


		ImGui::ColorEdit4("Obj Color", m_Color);
		ImGui::ColorEdit3("Light Color", m_LightColor);
		ImGui::SliderFloat3("Light Pos", &m_LightPos.x, -50.0f, 50.0f);



		ImGui::SliderFloat("Ambient Light", &m_AmbientIntensity, 0.0f, 3.0f);
		ImGui::SliderFloat("Specular Light", &m_SpecularIntensity, 0.00001f, 1.0f);


	}
}