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

    Sprite spr =
        Sprite(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f));

    Texture tex = Texture(640, 640);

    Framebuffer framebuffer = Framebuffer();
    framebuffer.AttachTexture(tex, FramebufferTarget::ReadWrite(), 0);

    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    // Loop until the user closes the window
    while (renderer.WindowOpen())
    {
        auto [frameBufferWidth, frameBufferHeight] =
            renderer.GetFrameBufferSize();

        float ratio = (float)frameBufferWidth / (float)frameBufferHeight;

        // This is recalculated each frame because the window can be resized

        glm::mat4 projection =
            glm::ortho(0.0f, (GLfloat)ratio, (GLfloat)1, 0.0f, 0.1f, 100.0f);

        renderer.PollEvents();

        // Firstly render to the texture
        // framebuffer.SetReadWriteTarget();
        framebuffer.BindReadWriteTarget();
        mat.SetInUse();
        tex.Use(0);
        mat.SetUniformPVM(projection, view, model);
        gl::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        renderer.DirectRender(Transform(model), spr.GetMesh(), mat);

        Framebuffer::BindDefaultFramebuffer();
        mat.SetInUse();
        tex.Use(0);
        mat.SetUniformPVM(projection, view, model);
        gl::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        renderer.DirectRender(Transform(model), spr.GetMesh(), mat);

        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}