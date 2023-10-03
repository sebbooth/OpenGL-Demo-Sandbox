#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace test {
	
	class Basic3D : public Test
	{
	public:
		Basic3D();
		~Basic3D();

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
		float m_Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glm::mat4 m_Model, m_View, m_Proj;
		glm::vec3 m_ViewTranslation, m_ModelTranslationA, m_ModelTranslationB;
	};
}


