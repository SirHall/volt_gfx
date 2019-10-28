#pragma once
#ifndef VOLT_GFX_RENDERER_HPP
#define VOLT_GFX_RENDERER_HPP

#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Transform.hpp"

#include <string>
#include <vector>

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
        int         bufferWidth = 0, bufferHeight = 0;
        GLFWwindow *window = nullptr;

    protected:
    public:
        Renderer();
        Renderer(const Renderer &other);
        Renderer(Renderer &&other);
        Renderer &operator=(const Renderer &other);
        Renderer &operator=(Renderer &&other);
        ~Renderer();

        bool Initialize(WindowCreationDataSettings windowSettings);

        // void SetRenderMode(RenderMode renderMode);

        void DirectRender(Transform transform, Mesh mesh, Shader shader);

        void InstancedRender(const std::vector<Transform> &transforms,
                             const Mesh &                  mesh);

        void DisplayFrame();
    };
} // namespace volt::gfx

#endif
