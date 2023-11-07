#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "OBJLoader.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace test {
	
	class ModelInstancing : public Test
	{
	public:
		ModelInstancing();
		~ModelInstancing();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr <OBJLoader> m_Model;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		GLFWwindow* m_Window = glfwGetCurrentContext();
		int m_Width, m_Height;

		float m_RotationY, m_RotationX;

		int m_NumInstances = 1000;

		glm::mat4 m_Proj, m_View, m_ModelMat;

		glm::vec3 m_CamDir = glm::vec3(0, 0, 1);
		glm::vec3 m_CamPos = glm::vec3(0, 60, -650);	// decent spawn point
		glm::vec3 m_CamUp = glm::vec3(0, 1, 0);
		glm::vec3 m_LightPos = glm::vec3(0, 100, -500);

		float m_AmbientCoeff = 0.25f;
		float m_DiffuseCoeff = 1.5f;
		float m_SpecularCoeff = 0.1f;

		float m_AmbientCol[3] = { 1.0f, 0.0f, 0.0f };
		float m_DiffuseCol[3] = { 0.0f, 1.0f, 0.0f };
		float m_SpecularCol[3] = { 0.0f, 0.0f, 1.0f };

		float m_ModelCol[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}


