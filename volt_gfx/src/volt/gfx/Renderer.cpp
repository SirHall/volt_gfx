#include "volt/gfx/Renderer.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/Shader.hpp"
#include "volt/gfx/global_events/GFXEvents.hpp"

#include "volt/event.hpp"

#include <iostream>
#include <thread>
#include <unordered_map>

using namespace volt::gfx;

// I doubt shared_ptr's are necessary, the raw pointers to Renderer
// TODO Should access to this be guarded with a read/write mutex?
std::unordered_map<GLFWwindow *, Renderer *> windowToRenderers =
    std::unordered_map<GLFWwindow *, Renderer *>();

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

Renderer::~Renderer() { this->Close(); }

void Renderer::SetupCallbacks()
{
    // Keyboard input callback
    glfwSetKeyCallback(this->window, [](GLFWwindow *window, int key,
                                        int scancode, int action, int mods) {
        auto keyEvent = GFXEventKey(windowToRenderers[window], key, scancode,
                                    KeyAction(action), mods);
        volt::event::global_event<GFXEventKey>::call_event(keyEvent);
    });

    glfwSetErrorCallback([](int errorCode, const char *errorMsg) {
        auto errorEvent = GFXEventError(errorCode, std::string(errorMsg));
        volt::event::global_event<GFXEventError>::call_event(errorEvent);
    });

    glfwSetCharCallback(
        this->window, [](GLFWwindow *window, unsigned int character) {
            auto charEvent = GFXEventChar(windowToRenderers[window], character);
            volt::event::global_event<GFXEventChar>::call_event(charEvent);
        });

    glfwSetMouseButtonCallback(this->window, [](GLFWwindow *window, int button,
                                                int action, int mods) {
        auto mouseButtonEvent =
            GFXEventMouseButton(windowToRenderers[window], MouseButton(button),
                                MouseButtonAction(action), mods);
        volt::event::global_event<GFXEventMouseButton>::call_event(
            mouseButtonEvent);
    });

    glfwSetCursorPosCallback(
        this->window, [](GLFWwindow *window, double x, double y) {
            auto cursorPosEvent =
                GFXEventCursorPos(windowToRenderers[window], x, y);
            volt::event::global_event<GFXEventCursorPos>::call_event(
                cursorPosEvent);
        });

    glfwSetWindowSizeCallback(
        this->window, [](GLFWwindow *window, int width, int height) {
            auto windowSizeEvent =
                GFXEventWindowSize(windowToRenderers[window], width, height);
            volt::event::global_event<GFXEventWindowSize>::call_event(
                windowSizeEvent);
        });

    glfwSetCursorEnterCallback(this->window, [](GLFWwindow *window,
                                                int         entered) {
        auto cursorEnterEvent = GFXEventCursorEnter(windowToRenderers[window],
                                                    entered == GLFW_TRUE);
        volt::event::global_event<GFXEventCursorEnter>::call_event(
            cursorEnterEvent);
    });

    glfwSetDropCallback(this->window, [](GLFWwindow *window, int path_count,
                                         const char *paths[]) {
        auto dropEvent =
            GFXEventDrop(windowToRenderers[window], path_count, paths);
        volt::event::global_event<GFXEventDrop>::call_event(dropEvent);
    });

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow *window,
                                                    int width, int height) {
        auto framebufferSizeEvent =
            GFXEventFramebufferSize(windowToRenderers[window], width, height);
        volt::event::global_event<GFXEventFramebufferSize>::call_event(
            framebufferSizeEvent);
    });

    glfwSetScrollCallback(
        this->window, [](GLFWwindow *window, double xoffset, double yoffset) {
            auto scrollEvent =
                GFXEventScroll(windowToRenderers[window], xoffset, yoffset);
            volt::event::global_event<GFXEventScroll>::call_event(scrollEvent);
        });

    glfwSetWindowCloseCallback(this->window, [](GLFWwindow *window) {
        auto windowCloseEvent = GFXEventWindowClose(windowToRenderers[window]);
        volt::event::global_event<GFXEventWindowClose>::call_event(
            windowCloseEvent);
    });

    glfwSetWindowContentScaleCallback(
        this->window, [](GLFWwindow *window, float xscale, float yscale) {
            auto windowContentScaleEvent = GFXEventWindowContentScale(
                windowToRenderers[window], xscale, yscale);
            volt::event::global_event<GFXEventWindowContentScale>::call_event(
                windowContentScaleEvent);
        });

    glfwSetWindowFocusCallback(this->window, [](GLFWwindow *window,
                                                int         focused) {
        auto windowFocusEvent = GFXEventWindowFocus(windowToRenderers[window],
                                                    focused == GLFW_TRUE);
        volt::event::global_event<GFXEventWindowFocus>::call_event(
            windowFocusEvent);
    });

    glfwSetWindowMaximizeCallback(
        this->window, [](GLFWwindow *window, int maximized) {
            auto windowMaximizeEvent = GFXEventWindowMaximize(
                windowToRenderers[window], maximized == GLFW_TRUE);
            volt::event::global_event<GFXEventWindowMaximize>::call_event(
                windowMaximizeEvent);
        });

    glfwSetWindowPosCallback(
        this->window, [](GLFWwindow *window, int xpos, int ypos) {
            auto windowPosEvent =
                GFXEventWindowPos(windowToRenderers[window], xpos, ypos);
            volt::event::global_event<GFXEventWindowPos>::call_event(
                windowPosEvent);
        });

    glfwSetWindowRefreshCallback(this->window, [](GLFWwindow *window) {
        auto windowRefreshEvent =
            GFXEventWindowRefresh(windowToRenderers[window]);
        volt::event::global_event<GFXEventWindowRefresh>::call_event(
            windowRefreshEvent);
    });

    //--- All available & unimplemented glfw callbacks ---//
    // glfwSetJoystickCallback
    // glfwSetMonitorCallback
    // glfwSetWindowIconifyCallback
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
    // Add this renderer to the static windowToRenderers unordered map
    windowToRenderers.insert_or_assign(window, this);

    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    gl::exts::LoadTest loaded = gl::sys::LoadFunctions();
    if (!loaded)
    {
        // The context does not work with the generated headers
        std::cerr << "Failed to load gl" << std::endl;
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
    // glewExperimental = GL_TRUE;

    // Turn VSync on
    GLCall(glfwSwapInterval(1));

    // Depth buffer
    gl::Enable(gl::DEPTH_TEST);

    // Setup viewport
    gl::Viewport(0, 0, bufferWidth, bufferHeight);

    this->SetupCallbacks();

    //--- OpenGL Code starts here ---//

    GLCall(std::cout << "OpenGL Driver: " << gl::GetString(gl::VERSION)
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
        std::cerr << "Attempted to perform a direct render with incomplete "
                     "data: vao: "
                  << vao << ", vbo: " << vbo << ", ibo: " << ibo << std::endl;
        return;
    }

    shader.SetInUse();

    GLCall(gl::BindVertexArray(vao));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, vbo)); // Probably not needed
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ibo));

    // The draw call
    GLCall(gl::DrawElements(gl::TRIANGLES, mesh.GetIndices().size(),
                            gl::UNSIGNED_INT, 0));

    GLCall(gl::BindVertexArray(0));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, 0)); // Probably not needed
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0));
}

// void Renderer::InstancedRender(const std::vector<Transform> &transforms,
//                                const Mesh &                  mesh)
// {
//     GLuint vao = mesh.GetVAO(), vbo = mesh.GetVBO(), ibo = mesh.GetIBO();
//     if (vao == 0 || vbo == 0 || ibo == 0)
//     {
//         std::cerr << "Attempted to direct render with incomplete data: vao: "
//                   << vao << ", vbo: " << vbo << ", ibo: " << ibo <<
//                   std::endl;
//         return;
//     }

//     GLCall(glBindVertexArray(vao));
//     // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); // Probably not needed
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

//     glBindVertexArray(mesh.GetVAO());

//     glDrawElementsInstanced(GL_TRIANGLES, mesh.GetIndices().size(),
//                             GL_UNSIGNED_INT, 0, transforms.size());

//     glBindVertexArray(0);

//     // The draw call
//     GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(),
//                           GL_UNSIGNED_INT, 0));

//     GLCall(glBindVertexArray(0));
//     // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Probably not needed
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
// }

void Renderer::DisplayFrame()
{
    GLCall(glfwSwapBuffers(window));
    GLCall(gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT));
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

void Renderer::Close()
{
    if (initialized)
    {
        windowToRenderers.erase(this->window);
        glfwSetWindowShouldClose(this->window, GLFW_TRUE);
        initialized = false;
    }
}
