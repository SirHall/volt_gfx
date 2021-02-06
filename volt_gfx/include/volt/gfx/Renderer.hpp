#pragma once
#ifndef VOLT_GFX_RENDERER_HPP
#define VOLT_GFX_RENDERER_HPP

#include "volt/gfx/Camera.hpp"
#include "volt/gfx/Framebuffer.hpp"
#include "volt/gfx/Material.hpp"
#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/RenderObject.hpp"
#include "volt/gfx/Transform.hpp"

#include <chrono>
#include <functional>
#include <string>
#include <tuple>
#include <vector>

using namespace std::chrono;

namespace volt::gfx
{
    enum class RenderMode
    {
        Points                 = GL_POINTS,
        Lines                  = GL_LINES,
        Triangles              = GL_TRIANGLES,
        LineStrip              = GL_LINE_STRIP,
        LineLoop               = GL_LINE_LOOP,
        LineStripAdjacency     = GL_LINE_STRIP_ADJACENCY,
        LinesAdjacency         = GL_LINES_ADJACENCY,
        TriangleStrip          = GL_TRIANGLE_STRIP,
        TriangleFan            = GL_TRIANGLE_FAN,
        TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        TrianglesAdjacency     = GL_TRIANGLES_ADJACENCY
    };

    struct GFXSettings
    {
        int         width = 640, height = 640;
        std::string title         = "";
        bool        multiSampling = false;
        bool        depthTest     = true;
        bool        blending      = true;
    };

    class Renderer
    {
    private:
        bool                     initialized = false;
        GLFWwindow *             window      = nullptr;
        float                    targetFPS   = 60.0f;
        float                    deltaTime   = 0.0f;
        steady_clock::time_point startTimePoint, frameTimePoint,
            lastFrameTimePoint;

        void SetupCallbacks();

        // Finds the monitor that this window is 'mainly' on, or the monitor
        // that most of the window's area is on
        GLFWmonitor *FindBestMonitor();

        bool autoResizeFrameBufs = false;

    public:
        Renderer();
        Renderer(const Renderer &other) = delete;
        Renderer(Renderer &&other)      = delete;
        Renderer &operator=(const Renderer &other) = delete;
        Renderer &operator                         =(Renderer &&other);
        ~Renderer();

        bool Initialize(GFXSettings settings);

        // void SetRenderMode(RenderMode renderMode);

        void DirectRender(RenderObject &obj, Transform const &transform,
                          Camera const &cam);

        void RenderFramebuffer(Framebuffer &fb, Material &mat,
                               std::uint8_t attachmentIndex = 0);

        void InstancedRender(RenderObject &                       obj,
                             std::vector<MeshInstanceData> const &transforms,
                             Camera const &                       cam);

        template <typename VertT, typename InstT>
        void InstancedRender(VAO<VertT, InstT> &vao, Shader shader,
                             RenderMode renderMode = RenderMode::Triangles)
        {
            vao.Bind();
            shader.Bind();
            GLCall(glDrawElementsInstanced(
                (GLenum)renderMode, (GLsizei)vao.GetIBO().Size(),
                GL_UNSIGNED_INT, 0, (GLsizei)vao.GetInstVBO().Size()));
        }

        template <typename VertT, typename InstT>
        void InstancedRender(VAO<VertT, InstT> &vao, Material mat,
                             Camera const &cam,
                             RenderMode    renderMode = RenderMode::Triangles)
        {
            mat.SetUniformPVM(cam.GetProjection(),
                              cam.GetTransform().GetMatrix(), glm::mat4(1.0));
            InstancedRender(vao, mat.GetShader(), renderMode);
        }

        void DisplayFrame();

        bool WindowOpen() const;

        void SetTargetFPS(float fps);

        float GetDeltaTime() const;

        float GetUpTime() const;

        void SleepForFrame();

        void PollEvents();

        void Close();

        std::tuple<int, int> GetWindowSize() const;

        void SetWindowSize(std::tuple<int, int> newSize);

        std::tuple<int, int> GetFrameBufferSize() const;

        void SetContextSize(std::tuple<int, int> newSize);

        void CorrectContextSize();

        float GetFrameBufferSizeRatio() const;

        bool IsFullscreen() const;

        void Fullscreen(bool goFullscreen);

        // If this is set to true, this renderer will automatically resize any
        // framebuffer being rendered to to match the size of the window.
        // This allows framebuffers to be automatically resized any time the
        // window is too.
        void SetAutoResizeFrameBufs(bool autoResize);

        bool GetAutoResizeFrameBufs() const;

        // Not a very good idea
        GLFWwindow *GetWindow();
    };
} // namespace volt::gfx

#endif
