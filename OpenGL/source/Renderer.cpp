#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] ("
            << error << "): "
            << function << " "
            << file << " : "
            << line << std::endl;
        return false;
    }
    return true;
}


void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
    GLCall(glClearColor(r, g, b, a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::updateProj(GLFWwindow* window, glm::mat4& proj, int& width, int& height, float FOV, float min, float max)
{
    // check window resizing
    int newWidth, newHeight;
    glfwGetFramebufferSize(window, &newWidth, &newHeight);
    if (newWidth != width || newHeight != height) {
        width = newWidth;
        height = newHeight;
        proj = glm::perspective(glm::radians(FOV), (float)width / (float)height, min, max);
        GLCall(glViewport(0, 0, width, height));
    }
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr, count));
}


