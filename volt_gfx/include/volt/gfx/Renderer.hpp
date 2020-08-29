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
    // enum RenderMode
    // {
    //     Direct,
    //     Instanced
    // };

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
        bool initialized = false;
        // RenderMode renderMode;
        // int                      bufferWidth = 0, bufferHeight = 0;
        GLFWwindow *             window    = nullptr;
        float                    targetFPS = 60.0f;
        float                    deltaTime = 0.0f;
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

        void DisplayFrame();

        bool WindowOpen();

        void SetTargetFPS(float fps);

        float GetDeltaTime();

        float GetUpTime();

        void SleepForFrame();

        void PollEvents();

        void Close();

        std::tuple<int, int> GetWindowSize();

        void SetWindowSize(std::tuple<int, int> newSize);

        std::tuple<int, int> GetFrameBufferSize();

        void SetContextSize(std::tuple<int, int> newSize);

        void CorrectContextSize();

        float GetFrameBufferSizeRatio();

        bool IsFullscreen();

        void Fullscreen(bool goFullscreen);

        // If this is set to true, this renderer will automatically resize any
        // framebuffer being rendered to to match the size of the window.
        // This allows framebuffers to be automatically resized any time the
        // window is too.
        void SetAutoResizeFrameBufs(bool autoResize);

        bool GetAutoResizeFrameBufs();
    };
} // namespace volt::gfx

#endif
