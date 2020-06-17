#include "volt/gfx/Renderer.hpp"
#include "volt/gfx/Camera.hpp"
#include "volt/gfx/Framebuffer.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/RenderObject.hpp"
#include "volt/gfx/Shader.hpp"
#include "volt/gfx/Sprite.hpp"
#include "volt/gfx/global_events/GFXEvents.hpp"

#include "volt/event.hpp"

#include <iostream>
#include <thread>
#include <unordered_map>

#include "volt/gfx/extern/stb_image.h"
#include "volt/gfx/extern/stb_image_write.h"

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
    this->initialized = other.initialized;
    this->window      = other.window;
    other.window      = nullptr;
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

    glfwSetWindowIconifyCallback(this->window, [](GLFWwindow *window,
                                                  int         iconified) {
        auto windowIconify = GFXEventWindowIconify(windowToRenderers[window],
                                                   iconified == GLFW_TRUE);
        volt::event::global_event<GFXEventWindowIconify>::call_event(
            windowIconify);
    });

    glfwSetJoystickCallback([](int jid, int event) {
        auto joystickEvent = GFXEventJoystick(jid, JoystickEvent(event));
        volt::event::global_event<GFXEventJoystick>::call_event(joystickEvent);
    });

    // TODO: Make this safer
    glfwSetMonitorCallback([](GLFWmonitor *monitor, int event) {
        auto monitorEvent = GFXEventMonitor(monitor, MonitorEvent(event));
        volt::event::global_event<GFXEventMonitor>::call_event(monitorEvent);
    });
}

// Modified version of: https://stackoverflow.com/a/31526753/13165325
GLFWmonitor *Renderer::FindBestMonitor()
{
    // If this is fullscreen, just return the monitor this is fullscreen on
    if (IsFullscreen())
        return glfwGetWindowMonitor(this->window);

    GLFWmonitor *primaryMon = nullptr;

    int bestoverlap = 0;

    int wx = 0, wy = 0, ww = 0, wh = 0;
    glfwGetWindowPos(this->window, &wx, &wy);
    glfwGetWindowSize(this->window, &ww, &wh);
    int           nmonitors = 0;
    GLFWmonitor **monitors  = glfwGetMonitors(&nmonitors);

    for (int i = 0; i < nmonitors; i++)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(monitors[i]);
        int                mx = 0, my = 0;
        glfwGetMonitorPos(monitors[i], &mx, &my);
        int mw = mode->width;
        int mh = mode->height;

        int overlap =
            std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
            std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap)
        {
            bestoverlap = overlap;
            primaryMon  = monitors[i];
        }
    }

    return primaryMon;
}

bool Renderer::Initialize(GFXSettings settings)
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
    // glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    if (settings.multiSampling)
        glfwWindowHint(GLFW_SAMPLES, 4); // Multi-sampling
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(settings.width, settings.height,
                              settings.title.c_str(), NULL, NULL);
    // Add this renderer to the static windowToRenderers unordered map
    windowToRenderers.insert_or_assign(window, this);

    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    // glexts::LoadTest loaded = glsys::LoadFunctions();
    // if (!loaded)
    // {
    //     // The context does not work with the generated headers
    //     std::cerr << "Failed to load gl" << std::endl;
    //     glfwTerminate();
    //     return false;
    // }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize glew
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // Only after the contect has been made current can we use glGetError()
    // calls on Windows

    // Allow experimental features
    // glewExperimental = GL_TRUE;

    // Turn VSync on
    // glfwSwapInterval(1);

    // Enable multi-sampling
    if (settings.multiSampling)
        GLCall(glEnable(GL_MULTISAMPLE));

    // Enable Depth buffer
    if (settings.depthTest)
        GLCall(glEnable(GL_DEPTH_TEST));

    // Enable blending
    if (settings.blending)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));

    auto [bufferWidth, bufferHeight] = this->GetFrameBufferSize();
    // Setup viewport
    GLCall(glViewport(0, 0, bufferWidth, bufferHeight));

    this->SetupCallbacks();

    //--- OpenGL Code starts here ---//

    GLCall(std::cout << "OpenGL Driver: " << glGetString(GL_VERSION)
                     << std::endl);

    initialized = true;

    // startOfThisFrameTimePoint = steady_clock::now();
    this->startTimePoint = this->lastFrameTimePoint = this->frameTimePoint =
        steady_clock::now();
    return true;
}

// void Renderer::SetRenderMode(RenderMode renderMode)
// {
//     this->renderMode = renderMode;
// }

void Renderer::DirectRender(RenderObject &obj, Camera const &cam)
{
    if (!obj.GetMesh().IsValid())
    {
        std::cerr << "Attempted to perform a direct render with incomplete "
                     "data: vao: "
                  << obj.GetMesh().GetVAO()
                  << ", vbo: " << obj.GetMesh().GetVBO()
                  << ", ibo: " << obj.GetMesh().GetIBO() << std::endl;
        return;
    }

    obj.Bind();
    obj.GetMaterial().SetUniformPVM(cam.GetProjection(),
                                    cam.GetTransform().GetMatrix(),
                                    obj.GetTransform().GetMatrix());
    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES,
                          (GLsizei)obj.GetMesh().GetIndices().size(),
                          GL_UNSIGNED_INT, 0));
}

void Renderer::RenderFramebuffer(Framebuffer &fb, Material &mat,
                                 std::uint8_t attachmentIndex)
{
    glm::mat4 proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    mat.Bind();
    mat.SetUniformPVM(proj, glm::mat4(1.0f), glm::mat4(1.0f));

    Mesh mesh = Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    mesh.Bind();

    fb.BindReadWriteTarget(attachmentIndex);
    auto tex                   = fb.GetTexture(attachmentIndex);
    auto [winWidth, winHeight] = this->GetFrameBufferSize();
    // Resize the texture if necessary
    if (this->autoResizeFrameBufs &&
        (tex->GetWidth() != winWidth || tex->GetHeight() != winHeight))
        tex->Resize(winWidth, winHeight);
    if (tex)
        this->SetContextSize(
            std::make_tuple(tex->GetWidth(), tex->GetHeight()));

    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetIndices().size(),
                          GL_UNSIGNED_INT, 0));

    mesh.Unbind();
    Framebuffer::BindDefaultFramebuffer();
    this->SetContextSize(this->GetFrameBufferSize());
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
    glfwSwapBuffers(window);
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

bool Renderer::WindowOpen() { return !glfwWindowShouldClose(window); }

// GetTime implemented just to make time handling here easier
float GetTime(steady_clock::time_point start, steady_clock::time_point now)
{
    using ms = std::chrono::duration<float, std::milli>;
    return std::chrono::duration_cast<ms>(now - start).count() / 1000.0f;
}

float GetTime(steady_clock::time_point start)
{
    return GetTime(start, steady_clock::now());
}

void Renderer::SetTargetFPS(float fps) { targetFPS = fps; }

float Renderer::GetDeltaTime()
{
    return GetTime(this->lastFrameTimePoint, this->frameTimePoint);
}

float Renderer::GetUpTime() { return GetTime(this->startTimePoint); }

void Renderer::SleepForFrame()
{

    // The amount of time left for this frame in seconds

    this->lastFrameTimePoint = this->frameTimePoint;
    this->frameTimePoint     = steady_clock::now();

    float timeLeft = (1.0f / targetFPS) -
                     (GetTime(this->lastFrameTimePoint, this->frameTimePoint));

    // Only sleep if we are running ahead of schedule
    // if (timeLeft > 0.0f)
    //     std::this_thread::sleep_for(
    //         std::chrono::milliseconds((int)(1000.0f * timeLeft)));
    // this->frameTimePoint = steady_clock::now();
}

void Renderer::PollEvents()
{
    // Poll for and process events
    glfwPollEvents();
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

std::tuple<int, int> Renderer::GetWindowSize()
{
    int width = 0, height = 0;
    glfwGetWindowSize(this->window, &width, &height);
    return std::make_tuple(width, height);
}

void Renderer::SetWindowSize(std::tuple<int, int> newSize)
{
    auto [width, height] = newSize;
    glfwSetWindowSize(this->window, width, height);
}

std::tuple<int, int> Renderer::GetFrameBufferSize()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(this->window, &width, &height);
    return std::make_tuple(width, height);
}

void Renderer::SetContextSize(std::tuple<int, int> newSize)
{
    auto [width, height] = newSize;
    glViewport(0, 0, width, height);
}

void Renderer::CorrectContextSize()
{
    auto [width, height] = this->GetFrameBufferSize();
    glViewport(0, 0, width, height);
}

float Renderer::GetFrameBufferSizeRatio()
{
    auto [width, height] = this->GetFrameBufferSize();
    return (float)width / (float)height;
}

bool Renderer::IsFullscreen()
{
    return glfwGetWindowMonitor(this->window) != nullptr;
}

void Renderer::Fullscreen(bool goFullscreen)
{
    if (IsFullscreen() == goFullscreen)
        return;

    GLFWmonitor *      monitor = FindBestMonitor();
    GLFWvidmode const *mode    = glfwGetVideoMode(monitor);

    if (goFullscreen)
        glfwSetWindowMonitor(this->window, monitor, 0, 0, mode->width,
                             mode->height, 0);
    else
        glfwSetWindowMonitor(this->window, nullptr, 0, 0, mode->width,
                             mode->height, 0);
}

void Renderer::SetAutoResizeFrameBufs(bool autoResize)
{
    this->autoResizeFrameBufs = autoResize;
}

bool Renderer::GetAutoResizeFrameBufs() { return this->autoResizeFrameBufs; }