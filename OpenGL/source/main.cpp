#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture2DBatch.h"
#include "tests/Basic3D.h"
#include "tests/ModelLoading.h"


int main(void)
{
    ////////////////////////////    GLFW Initialization     ////////////////////////////
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);     // Make the window's context current 
    glfwSwapInterval(0);    //limit fps -- set 1 for vsync, set 0 for max
    ////////////////////////////////////////////////////////////////////////////////////



    ////////////////////////////    GLEW Initialization     ////////////////////////////
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Status: Using GL %s\n", glGetString(GL_VERSION));
    ////////////////////////////////////////////////////////////////////////////////////


    {
        Renderer renderer;
        
        ImGui::CreateContext();     // ImGui Initialization
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();
        
        test::Test* currentTest = nullptr;      // Test menu initialization
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");    // Add tests
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");    // Add tests
        testMenu->RegisterTest<test::TestTexture2DBatch>("2D Texture Batch");    // Add tests
        testMenu->RegisterTest<test::Basic3D>("Basic 3D");    // Add tests
        testMenu->RegisterTest<test::ModelLoading>("Model Loading");    // Add tests

        while (!glfwWindowShouldClose(window))
        {
            renderer.setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        if (currentTest != testMenu)
            delete testMenu;
        delete currentTest;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    
    _CrtDumpMemoryLeaks();
    return 0;
}  