#pragma once
#ifndef VOLT_GFX_RENDERER_HPP
#define VOLT_GFX_RENDERER_HPP

#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Shader.hpp"
#include "volt/gfx/Transform.hpp"

#include <chrono>
#include <functional>
#include <string>
#include <vector>

using namespace std::chrono;

namespace volt::gfx
{
    // enum RenderMode
    // {
    //     Direct,
    //     Instanced
    // };

    struct WindowCreationDataSettings
    {
        int         width = 640, height = 640;
        std::string title = "";
    };

    class Renderer
    {
    private:
        bool initialized = false;
        // RenderMode renderMode;
        int                      bufferWidth = 0, bufferHeight = 0;
        GLFWwindow *             window    = nullptr;
        float                    targetFPS = 60.0f;
        float                    deltaTime = 0.0f;
        steady_clock::time_point endOfLastFrameTimePoint,
            startOfThisFrameTimePoint;

    public:
        Renderer();
        Renderer(const Renderer &other) = delete;
        Renderer(Renderer &&other)      = delete;
        Renderer &operator=(const Renderer &other) = delete;
        Renderer &operator                         =(Renderer &&other);
        ~Renderer();

        bool Initialize(WindowCreationDataSettings windowSettings);

        // void SetRenderMode(RenderMode renderMode);

        void DirectRender(const Transform &transform, const Mesh &mesh,
                          const Shader &shader);

        // void InstancedRender(const std::vector<Transform> &transforms,
        //                      const Mesh &                  mesh);

        void DisplayFrame();

        bool WindowOpen();

        int GetBufferWidth();

        int GetBufferHeight();

        void SetTargetFPS(float fps);

        float GetDeltaTime();

        void SleepForFrame();

        void PollEvents();

        void Close();
    };
} // namespace volt::gfx

#endif
