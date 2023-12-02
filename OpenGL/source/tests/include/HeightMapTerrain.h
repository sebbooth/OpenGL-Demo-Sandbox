#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace test {
	
	class HeightMapTerrain : public Test
	{
	public:
		HeightMapTerrain();
		~HeightMapTerrain();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		GLFWwindow* m_Window = glfwGetCurrentContext();

		int m_Width, m_Height;

		float m_RotationY, m_RotationX;
		float m_MaxHeight = 70.;
		float m_Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		bool m_Wireframe = false;

		glm::mat4 m_Model, m_View, m_Proj;

		glm::vec3 m_CamDir = glm::vec3(0, 0, 1);
		glm::vec3 m_CamPos = glm::vec3(0, 30, -200);	// decent spawn point
		glm::vec3 m_CamUp = glm::vec3(0, 1, 0);
	};
}


