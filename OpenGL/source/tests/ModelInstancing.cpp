#include "include/ModelInstancing.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>

namespace test {


	ModelInstancing::ModelInstancing()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f))
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
		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);
		m_Shader->SetUniform1f("u_NumInstances", (float)m_NumInstances);

		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);
	}

	ModelInstancing::~ModelInstancing()
	{
	}

	void ModelInstancing::OnUpdate(float deltaTime)
	{
		float moveSpeed = 0.1f;
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

	void ModelInstancing::OnRender()
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
		m_Shader->SetUniformMat4f("u_Mod", m_ModelMat);
		m_Shader->SetUniformVec3("u_ViewPos", m_CamPos);
		m_Shader->SetUniformVec3("u_LightPos", m_LightPos);

		m_Shader->SetUniform1f("u_InstancesSqrt", sqrt((float)m_NumInstances));

		m_Shader->SetUniform1f("u_AmbientCoeff", m_AmbientCoeff);
		m_Shader->SetUniform1f("u_DiffuseCoeff", m_DiffuseCoeff);
		m_Shader->SetUniform1f("u_SpecularCoeff", m_SpecularCoeff);
		m_Shader->SetUniform3f("u_AmbientCol", m_AmbientCol[0], m_AmbientCol[1], m_AmbientCol[2]);
		m_Shader->SetUniform3f("u_DiffuseCol", m_DiffuseCol[0], m_DiffuseCol[1], m_DiffuseCol[2]);
		m_Shader->SetUniform3f("u_SpecularCol", m_SpecularCol[0], m_SpecularCol[1], m_SpecularCol[2]);
		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);

		renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, m_NumInstances);


	}

	void ModelInstancing::OnImGuiRender()
	{
		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::SliderInt("Num Instances", &m_NumInstances, 0.0f, 10000.0f);
		ImGui::ColorEdit4("Obj Color", m_ModelCol);
		ImGui::SliderFloat3("Light Pos", &m_LightPos.x, -500.0f, 500.0f);
		ImGui::SliderFloat("Ambient Light", &m_AmbientCoeff, 0.0f, 2.0f);
		ImGui::SliderFloat("Diffuse Light", &m_DiffuseCoeff, 0.0f, 2.0f);
		ImGui::SliderFloat("Specular Light", &m_SpecularCoeff, 0.0f, 2.0f);
		ImGui::ColorEdit3("Ambient Color", m_AmbientCol);
		ImGui::ColorEdit3("Diffuse Color", m_DiffuseCol);
		ImGui::ColorEdit3("Specular Color", m_SpecularCol);
	}
}