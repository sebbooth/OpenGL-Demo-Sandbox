#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

