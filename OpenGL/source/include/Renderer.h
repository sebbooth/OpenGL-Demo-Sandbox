#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define ASSERT(x) if (!(x))  __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//#define GLCall(x) x
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{

public:
	void Clear() const;
	void setClearColor(float r, float g, float b, float a);
	void updateProj(GLFWwindow* window, glm::mat4& proj, int& width, int& height, float FOV, float min, float max);
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) const;
};

