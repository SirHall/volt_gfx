#include "volt/gfx/Renderer.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/Shader.hpp"

// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End

#include <iostream>
#include <thread>

using namespace volt::gfx;

Renderer::Renderer() {}

// Renderer::Renderer(const Renderer &other) {}

// Renderer::Renderer(Renderer &&other) {}

// Renderer &Renderer::operator=(const Renderer &other) { return *this; }

Renderer &Renderer::operator=(Renderer &&other)
{
    this->bufferHeight = other.bufferHeight;
    this->bufferWidth  = other.bufferWidth;
    this->initialized  = other.initialized;
    this->window       = other.window;
    other.window       = nullptr;
    return *this;
}

Renderer::~Renderer()
{
    if (initialized)
    {
        glfwTerminate();
        initialized = false;
    }
}

bool Renderer::Initialize(WindowCreationDataSettings windowSettings)
{

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW, quitting." << std::endl;
        glfwTerminate();
        return false;
    }

    // Tell OpenGL to use GLSL 3.30
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWmonitor *      monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode    = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(windowSettings.width, windowSettings.height,
                              windowSettings.title.c_str(), NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Get window buffer size
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Only after the contect has been made current can we use glGetError()
    // calls on Windows

    // Allow experimental features
    glewExperimental = GL_TRUE;

    // Turn VSync on
    GLCall(glfwSwapInterval(1));

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

    startOfThisFrameTimePoint = steady_clock::now();
    endOfLastFrameTimePoint   = steady_clock::now();
    return true;
}

// void Renderer::SetRenderMode(RenderMode renderMode)
// {
//     this->renderMode = renderMode;
// }

void Renderer::DirectRender(const Transform &transform, const Mesh &mesh,
                            const Shader &shader)
{
    GLuint vao = mesh.GetVAO(), vbo = mesh.GetVBO(), ibo = mesh.GetIBO();
    if (vao == 0 || vbo == 0 || ibo == 0)
    {
        std::cerr << "Attempted to direct render with incomplete data: vao: "
                  << vao << ", vbo: " << vbo << ", ibo: " << ibo << std::endl;
        return;
    }

    shader.SetInUse();

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); // Probably not needed
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(),
                          GL_UNSIGNED_INT, 0));

    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Probably not needed
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Renderer::InstancedRender(const std::vector<Transform> &transforms,
                               const Mesh &                  mesh)
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

    glBindVertexArray(mesh.GetVAO());

    glDrawElementsInstanced(GL_TRIANGLES, mesh.GetIndices().size(),
                            GL_UNSIGNED_INT, 0, transforms.size());

    glBindVertexArray(0);

    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(),
                          GL_UNSIGNED_INT, 0));

    GLCall(glBindVertexArray(0));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Probably not needed
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Renderer::DisplayFrame()
{
    GLCall(glfwSwapBuffers(window));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

bool Renderer::WindowOpen() { return !glfwWindowShouldClose(window); }

int Renderer::GetBufferWidth() { return bufferWidth; }

int Renderer::GetBufferHeight() { return bufferHeight; }

// GetTime implemented just to make time handling here easier
float GetTime(steady_clock::time_point start, steady_clock::time_point now)
{
    steady_clock::duration duration = now - start;
    return float(duration.count()) * steady_clock::period::num /
           steady_clock::period::den;
}

float GetTime(steady_clock::time_point start)
{
    return GetTime(start, steady_clock::now());
}

void Renderer::SetTargetFPS(float fps) { targetFPS = fps; }

float Renderer::GetDeltaTime() { return GetTime(endOfLastFrameTimePoint); }

void Renderer::SleepForFrame()
{
    endOfLastFrameTimePoint = steady_clock::now();

    // The amount of time left for this frame in seconds
    float timeLeft = (1.0f / targetFPS) - (GetTime(startOfThisFrameTimePoint,
                                                   endOfLastFrameTimePoint));

    // Only sleep if we are running ahead of schedule
    if (timeLeft > 0.0f)
        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(1000 * timeLeft)));

    startOfThisFrameTimePoint = steady_clock::now();
}

void Renderer::PollEvents()
{
    // Poll for and process events
    GLCall(glfwPollEvents());
}