#include "Basic3D.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	Basic3D::Basic3D()
		: m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Proj(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,0,-10)), 
		m_ModelTranslationA(glm::vec3(-2, 0, 0)), m_ModelTranslationB(glm::vec3(2, 0, 0))
	{
		float positions[] = {
			//  x,     y,     z,   tex.x, tex.y,
			-1.0f, -1.0f, -1.0f,	0.0f,  0.0f,	// 0  bot left  back
			 1.0f, -1.0f, -1.0f,	1.0f,  0.0f,	// 1  bot right back
			 1.0f,  1.0f, -1.0f,	1.0f,  1.0f,	// 2  top right back
			-1.0f,  1.0f, -1.0f,	0.0f,  1.0f,	// 3  top left  back

			-1.0f, -1.0f,  1.0f,	1.0f,  0.0f,	// 4  bot left  front
			 1.0f, -1.0f,  1.0f,	0.0f,  0.0f,	// 5  bot right front
			 1.0f,  1.0f,  1.0f,	0.0f,  1.0f,	// 6  top right front
			-1.0f,  1.0f,  1.0f,	1.0f,  1.0f,	// 7  top left  front

			 1.0f,  1.0f,  1.0f,	0.0f,  0.0f,	// 8  top right front
			-1.0f,  1.0f,  1.0f,	1.0f,  0.0f,	// 9  top left  front
			-1.0f, -1.0f,  1.0f,	1.0f,  1.0f,	// 10  bot left  front
			 1.0f, -1.0f,  1.0f,	0.0f,  1.0f,	// 11  bot right front

		};

		unsigned int indices[] = {
		   0,1,2,	//back
		   2,3,0,

		   4,5,6,	//front
		   6,7,4,

		   0,3,7,	//left
		   7,4,0,

		   1,2,6,	//right
		   6,5,1,

		   0,1,11,	//bot
		   11,10,0,

		   2,3,9,	//top
		   9,8,2
		};

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 12 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Shader = std::make_unique<Shader>("resources/shaders/Basic.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

		m_Texture = std::make_unique<Texture>("resources/textures/brick.JPG");

		m_Shader->SetUniform1i("u_Texture", 0);

		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
	}

	Basic3D::~Basic3D()
	{
	}

	void Basic3D::OnUpdate(float deltaTime)
	{
	}

	void Basic3D::OnRender()
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

		m_Texture->Bind();

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		{
			m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
			m_Model = glm::rotate(m_Model, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
			m_Model = glm::translate(m_Model, m_ModelTranslationA);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
			m_Model = glm::rotate(m_Model, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
			m_Model = glm::translate(m_Model, m_ModelTranslationB);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void Basic3D::OnImGuiRender()
	{

		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslationA.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Model B", &m_ModelTranslationB.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::ColorEdit4("Color B", m_Color);


	}
}