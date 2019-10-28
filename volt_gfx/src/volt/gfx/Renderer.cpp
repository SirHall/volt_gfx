#include "volt/gfx/Renderer.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/Shader.hpp"

// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End

#include <iostream>

using namespace volt::gfx;

Renderer::Renderer() {}

Renderer::Renderer(const Renderer &other) {}

Renderer::Renderer(Renderer &&other) {}

Renderer &Renderer::operator=(const Renderer &other) { return *this; }

Renderer &Renderer::operator=(Renderer &&other) { return *this; }

Renderer::~Renderer()
{
    if (initialized)
    {
        GLCall(glfwTerminate());
        initialized = false;
    }
}

bool Renderer::Initialize(WindowCreationDataSettings windowSettings)
{

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW, quitting." << std::endl;
        GLCall(glfwTerminate());
        return false;
    }

    // Tell OpenGL to use GLSL 3.30
    GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
    GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
    GLCall(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));
    GLCall(glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE));

    GLFWmonitor *      monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode    = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLCall(glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE));

    // Create a windowed mode window and its OpenGL context
    GLCall(window =
               glfwCreateWindow(windowSettings.width, windowSettings.height,
                                windowSettings.title.c_str(), NULL, NULL));

    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        GLCall(glfwTerminate());
        return false;
    }

    // Get window buffer size
    GLCall(glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight));

    // Make the window's context current
    GLCall(glfwMakeContextCurrent(window));

    // Allow experimental features
    glewExperimental = GL_TRUE;

    // Turn VSync on
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    { /* Problem: glewInit failed, something is seriously wrong. */
        GLCall(fprintf(stderr, "Error: %s\n", glewGetErrorString(err)));
        GLCall(glfwDestroyWindow(window));
        GLCall(glfwTerminate());
        return 1;
    }

    // Depth buffer
    glEnable(GL_DEPTH_TEST);

    // Setup viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    GLCall(fprintf(stdout, "Status: Using GLEW %s\n",
                   glewGetString(GLEW_VERSION)));

    //--- OpenGL Code starts here ---//

    GLCall(std::cout << "OpenGL Version: " << glGetString(GL_VERSION)
                     << std::endl);
    initialized = true;
    return true;
}

// void Renderer::SetRenderMode(RenderMode renderMode)
// {
//     this->renderMode = renderMode;
// }

void Renderer::DirectRender(Transform transform, Mesh mesh, Shader shader)
{
    GLuint vao = mesh.GetVAO(), vbo = mesh.GetVBO(), ibo = mesh.GetIBO();
    if (vao == 0 || vbo == 0 || ibo == 0)
    {
        std::cerr << "Attempted to direct render with incomplete data: vao: "
                  << vao << ", vbo: " << vbo << ", ibo: " << ibo << std::endl;
        return;
    }

    GLCall(glBindVertexArray(vao));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); // Probably not needed
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(),
                          GL_UNSIGNED_INT, 0));

    GLCall(glBindVertexArray(0));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Probably not needed
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Renderer::InstancedRender(const std::vector<Transform> &transforms,
                               const Mesh &                  mesh)
{
}

void Renderer::DisplayFrame()
{
    GLCall(glfwSwapBuffers(window));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}