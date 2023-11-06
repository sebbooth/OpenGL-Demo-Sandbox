#include "Grass.h"

#include "imgui/imgui.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "TerrainGrid.h"

namespace test {


	Grass::Grass()
		: m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)),
		m_ViewTranslation(glm::vec3(0,-5,-45)),
		m_ModelTranslation(glm::vec3(0,-4,-10))
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_MULTISAMPLE));

		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		m_Model = std::make_unique<OBJLoader>("resources/models/obj/grassBlade/grassBlade.obj");
		m_Texture = std::make_unique<Texture>("resources/textures/heightmap1.png");

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Model->m_Vertices.data(), m_Model->m_Vertices.size() * sizeof(float) * 8);
		
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Model->m_Indices.data(), m_Model->m_Indices.size());

		m_Shader = std::make_unique<Shader>("resources/shaders/BasicGrass.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);

		m_RotationX = 6.5f;
		m_Proj = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 1000000.0f);

			/*
			// Plane

			float positions[] = {
				//  x,     y,			     z,	  nx,   ny,   nz,  tex.x, tex.y,
				-100000.0f,  0.0,   -100000.0f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,	// 0  left  back
				 100000.0f,  0.0f,  -100000.0f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,	// 1  right back
				 100000.0f,  0.0f,   100000.0f,	0.0f, 1.0f, 0.0f,	1.0f,  1.0f,	// 2  right back
				-100000.0f,  0.0f,   100000.0f,	0.0f, 1.0f, 0.0f,	0.0f,  1.0f,	// 3  left  back
			};

			unsigned int indices[] = {
			   0,1,2,
			   2,3,0,
			};

			m_PlaneVAO = std::make_unique<VertexArray>();
			m_PlaneVertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 8 * sizeof(float));

			VertexBufferLayout planeLayout;
			planeLayout.Push<float>(3);
			planeLayout.Push<float>(3);
			planeLayout.Push<float>(2);
			m_PlaneVAO->AddBuffer(*m_PlaneVertexBuffer, planeLayout);

			m_PlaneIndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

			m_PlaneShader = std::make_unique<Shader>("resources/shaders/BasicGrassPlane.glsl");
			m_PlaneShader->Bind();
			m_PlaneShader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);

			m_PlaneTexture = std::make_unique<Texture>("resources/textures/brick.JPG");		
			*/
		
		float gridSize = 1.0f;
		int gridNum = 300;
		TerrainGrid grid = TerrainGrid(gridNum, gridNum, gridSize);

		m_PlaneVAO = std::make_unique<VertexArray>();
		m_PlaneVertexBuffer = std::make_unique<VertexBuffer>(grid.m_Vertices.data(), grid.m_Vertices.size() * sizeof(float) * 3);
		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3);
		m_PlaneVAO->AddBuffer(*m_PlaneVertexBuffer, planeLayout);

		m_PlaneIndexBuffer = std::make_unique<IndexBuffer>(grid.m_Indices.data(), grid.m_Indices.size());

		m_PlaneShader = std::make_unique<Shader>("resources/shaders/Terrain.glsl");

		m_PlaneShader->Bind();


		m_PlaneShader->SetUniform1i("u_Texture", 0);
		m_PlaneShader->SetUniform1f("u_GridSize", gridSize);
		m_PlaneShader->SetUniform1f("u_GridW", (float)gridNum);
		m_PlaneShader->SetUniform1f("u_GridL", (float)gridNum);
		m_PlaneShader->SetUniform4f("u_Color", m_PlaneCol[0], m_PlaneCol[1], m_PlaneCol[2], m_PlaneCol[3]);
		
	}

	Grass::~Grass()
	{
	}

	void Grass::OnUpdate(float deltaTime)
	{
		m_Time += deltaTime;
	}

	void Grass::OnRender()
	{
		Renderer renderer;

		renderer.updateProj(m_Window, m_Proj, m_Width, m_Height, 45.0f, 0.1f, 100000.0f);
		renderer.Clear();


		m_Texture->Bind();

		m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
		m_ModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMat = glm::rotate(m_ModelMat, glm::radians(m_RotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMat = glm::translate(m_ModelMat, m_ModelTranslation);
		glm::mat4 mvp = m_Proj * m_View * m_ModelMat;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniformMat4f("u_Mod", m_ModelMat);
		m_Shader->SetUniformVec3("u_ViewPos", m_ViewTranslation);

		m_Shader->SetUniform1f("u_Time", m_Time);
		m_Shader->SetUniform1f("u_FogDist", m_FogDist);

		m_Shader->SetUniform4f("u_ModelCol", m_ModelCol[0], m_ModelCol[1], m_ModelCol[2], m_ModelCol[3]);


		renderer.setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, 300*300);




		m_PlaneShader->Bind();
		m_PlaneShader->SetUniformMat4f("u_MVP", mvp);
		m_PlaneShader->SetUniform1f("u_FogDist", m_FogDist);
		m_PlaneShader->SetUniform4f("u_Color", m_PlaneCol[0], m_PlaneCol[1], m_PlaneCol[2], m_PlaneCol[3]);

		renderer.Draw(*m_PlaneVAO, *m_PlaneIndexBuffer, *m_PlaneShader);

	}

	void Grass::OnImGuiRender()
	{
		ImGui::SliderFloat3("View", &m_ViewTranslation.x, -300.0f, 300.0f);
		ImGui::SliderFloat3("Model A", &m_ModelTranslation.x, -300.0f, 300.0f);

		ImGui::SliderFloat("Rotate abt Y", &m_RotationY, -200.0f, 200.0f);
		ImGui::SliderFloat("Rotate abt X", &m_RotationX, -200.0f, 200.0f);
		ImGui::SliderFloat("Fog", &m_FogDist, 0.0f, 2000.0f);

		ImGui::ColorEdit4("Obj Color", m_ModelCol);
		ImGui::ColorEdit4("Plane Color", m_PlaneCol);
	}
}