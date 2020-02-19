#include "volt/event.hpp"
#include "volt/gfx.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

using namespace volt::gfx;

int main(int argc, char *argv[])
{
#pragma region Setup Renderer and Window

    auto renderer = Renderer();

    auto settings          = GFXSettings();
    settings.height        = 640;
    settings.width         = 640;
    settings.title         = "FrameBuffer Test";
    settings.blending      = true;
    settings.multiSampling = true;
    settings.depthTest     = true;

    if (!renderer.Initialize(settings))
    {
        std::cout << "Failed to initialize window" << std::endl;
        std::exit(1);
    }
    renderer.SetTargetFPS(60.0f);

    auto keyStrikeObserver =
        volt::event::observer<GFXEventKey>([&](GFXEventKey const &e) {
            if (e.GetAction().Get() == KeyAction::Press().Get())
            {
                char const *keyName =
                    glfwGetKeyName(e.GetKey(), e.GetScanCode());
                if (keyName != nullptr)
                    std::cout << '[' << keyName << ']' << std::endl;
            }

            if (e.GetKey() == GLFW_KEY_ESCAPE)
                renderer.Close();
        });

    // Resize the context when the frame buffer gets resized
    auto frameBufferResizeObserver =
        volt::event::observer<GFXEventFramebufferSize>(
            [&](GFXEventFramebufferSize &e) { renderer.CorrectContextSize(); });

#pragma endregion

#pragma region Compile Shaders

    auto           shaderSource = ShaderSource("fb");
    ShadeletSource shadeletVert =
        ShadeletSource("res/fb_vert.glsl", ShadeletType::Vertex);
    ShadeletSource shadeletFrag =
        ShadeletSource("res/fb_frag.glsl", ShadeletType::Fragment);
    shaderSource.AddShadelet(shadeletVert);
    shaderSource.AddShadelet(shadeletFrag);

    auto shaderCompileErrors = std::vector<std::string>();
    auto shader = Shader::CompileShader(shaderSource, shaderCompileErrors);

    std::cout << shader.GetProgram() << std::endl;

    if (shaderCompileErrors.size() > 0)
    {
        for (auto error : shaderCompileErrors)
            std::cerr << error << std::endl;
        exit(1);
    }
    else if (!shader.IsValid())
    {
        std::cerr << "Shader is invalid" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "All shaders compiled successfully" << std::endl;
    }

    Material mat = Material(shader);

#pragma endregion

    Texture tex = Texture(640, 640);

    Framebuffer framebuffer = Framebuffer();
    framebuffer.AttachTexture(tex, FramebufferTarget::ReadWrite(), 0);

    RenderObject obj =
        RenderObject(mat,
                     Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                        glm::vec2(1.0f, 1.0f)),
                     Transform(glm::translate(glm::mat4(1.0f),
                                              glm::vec3(0.5f, 0.5f, 0.0f))));

    Camera cam = Camera();
    cam.SetTransform(Transform(glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f))));
    cam.SetPerspectiveMode(false);
    cam.SetOrthoSize(1.0f);
    cam.SetNearFarPlanes(0.1f, 100.0f);
    // Loop until the user closes the window
    while (renderer.WindowOpen())
    {
        float ratio = renderer.GetFrameBufferSizeRatio();

        // This is recalculated each frame because the window can be resized

        renderer.PollEvents();

        cam.SetAspectRatio(renderer.GetFrameBufferSizeRatio());
        tex.Use(0);

        // Firstly render to the texture
        framebuffer.BindReadWriteTarget();
        renderer.DirectRender(obj, cam);

        Framebuffer::BindDefaultFramebuffer();
        renderer.DirectRender(obj, cam);

        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}