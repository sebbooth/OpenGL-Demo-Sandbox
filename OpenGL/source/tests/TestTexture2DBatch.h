#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {
	
	class TestTexture2DBatch : public Test
	{

	public:
		TestTexture2DBatch();
		~TestTexture2DBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;

		float m_PositionA[2] = {   0.0f,   0.0f };
		float m_PositionB[2] = { 100.0f, 100.0f };
		float m_ColorA[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
		float m_ColorB[4] = { 0.0f, 0.0f ,1.0f, 0.0f };

		glm::vec3 m_ViewTranslation;
		glm::mat4 m_Proj, m_View;

	};
}


