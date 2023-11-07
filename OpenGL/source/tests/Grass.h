#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "OBJLoader.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace test {
	
	class Grass : public Test
	{
	public:
		Grass();
		~Grass();

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

		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<IndexBuffer> m_PlaneIndexBuffer;
		std::unique_ptr<VertexBuffer> m_PlaneVertexBuffer;
		std::unique_ptr<Shader> m_PlaneShader;
		std::unique_ptr<Texture> m_PlaneTexture;

		GLFWwindow* m_Window = glfwGetCurrentContext();
		int m_Width, m_Height;

		float m_RotationY, m_RotationX;
		float m_Time = 0;
		float m_FogDist = 375.0f;
		int m_GridNum = 300 * 300;

		glm::mat4 m_Proj, m_View, m_ModelMat;

		float m_ModelCol[4] = { 0.0f, 0.8f, 0.0f, 1.0f };
		float m_PlaneCol[4] = { 0.0f, 0.05f, 0.0f, 1.0f };

		glm::vec3 m_CamDir = glm::vec3(0, 0, 1);
		glm::vec3 m_CamPos = glm::vec3(0, 30, -150);	// decent spawn point
		glm::vec3 m_CamUp = glm::vec3(0, 1, 0);
	};
}


