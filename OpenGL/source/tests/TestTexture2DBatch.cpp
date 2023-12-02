#include "include/TestTexture2DBatch.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>
#include <algorithm>

struct Vertex {
	float Position[2];
	float TexCoords[2];
	float TexID;
	float Color[4];
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float size, float textureID, float *color)
{
	Vertex v0 = { {x,      y     }, {0.0f, 0.0f}, textureID, { color[0], color[1], color[2], color[3] } };
	Vertex v1 = { {x+size, y     }, {1.0f, 0.0f}, textureID, { color[0], color[1], color[2], color[3] } };
	Vertex v2 = { {x+size, y+size}, {1.0f, 1.0f}, textureID, { color[0], color[1], color[2], color[3] } };
	Vertex v3 = { {x,      y+size}, {0.0f, 1.0f}, textureID, { color[0], color[1], color[2], color[3] } };

	return { v0,v1,v2,v3 };
}


namespace test {
	TestTexture2DBatch::TestTexture2DBatch()
		:m_ViewTranslation(0,0,0),
		m_View(glm::translate(glm::mat4(1.0f), m_ViewTranslation))
	{

		unsigned int indices[] = {
		   0,1,2,
		   2,3,0,
		   4,5,6,
		   6,7,4
		};

		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
		m_Proj = glm::ortho(0.0f, (float)m_Width, 0.0f, (float)m_Height, -1000.0f, 1000.0f);
		GLCall(glViewport(0, 0, m_Width, m_Height));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 1000 * sizeof(Vertex));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Push<float>(4);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("resources/shaders/BasicBatch.glsl");
		m_Shader->Bind();

		m_Texture1 = std::make_unique<Texture>("resources/textures/kitten.png");
		m_Texture2 = std::make_unique<Texture>("resources/textures/kitten2.png");
		
		int samplers[2] = { 0,1 };

		m_Shader->SetUniform1iv("u_Textures", 2, samplers);
	}

	TestTexture2DBatch::~TestTexture2DBatch()
	{
	}

	void TestTexture2DBatch::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2DBatch::OnRender()
	{
		//window resizing
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		if (width != m_Width || height != m_Height) {
			m_Width = width;
			m_Height = height;
			m_Proj = glm::ortho(0.0f, (float)m_Width, 0.0f, (float)m_Height, -1000.0f, 1000.0f);
			GLCall(glViewport(0, 0, m_Width, m_Height));
		}

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);
		
		float size = std::max({ m_Width / 8.0f, m_Height/5.0f });
		auto q0 = CreateQuad(m_PositionA[0], m_PositionA[1], size, 0.0f, m_ColorA);
		auto q1 = CreateQuad(m_PositionB[0], m_PositionB[1], size, 1.0f, m_ColorB);

		Vertex vertices[8];
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices+q0.size(), q1.data(), q1.size() * sizeof(Vertex));

		m_VertexBuffer->SubData(vertices, sizeof(vertices));

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		
		}
	}

	void TestTexture2DBatch::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation A", m_PositionA, 0.0f, 960.0f);
		ImGui::ColorEdit4("Color A", m_ColorA);
		ImGui::SliderFloat3("Translation B", m_PositionB, 0.0f, 960.0f);
		ImGui::ColorEdit4("Color B", m_ColorB);
	}
}