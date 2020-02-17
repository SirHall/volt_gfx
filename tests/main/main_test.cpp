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
    settings.title         = "VoltGFX";
    settings.blending      = true;
    settings.depthTest     = true;
    settings.multiSampling = true;

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

    // Get executable file path
    auto execPath = boost::filesystem::system_complete(argv[0]).parent_path();

    auto           shaderSource = ShaderSource("default");
    ShadeletSource shadeletVert =
        ShadeletSource("res/shader_vert.glsl", ShadeletType::Vertex);
    ShadeletSource shadeletFrag =
        ShadeletSource("res/shader_frag.glsl", ShadeletType::Fragment);
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

    Sprite spr1 =
        Sprite(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(5.0f, 5.0f));
    Sprite spr2 =
        Sprite(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(5.0f, 5.0f));

    // Load uv sprite
    Image   img = Image("res/tree.png");
    Texture tex = Texture(img);

    glm::mat4 model1 =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 model2 =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    // Loop until the user closes the window
    while (renderer.WindowOpen())
    {
        // Rotate model a bit
        model1 = glm::rotate(model1, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));

        auto [frameBufferWidth, frameBufferHeight] =
            renderer.GetFrameBufferSize();

        // This is recalculated each frame because the window can be resized
        glm::mat4 projection = glm::perspective(
            45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.1f,
            100.0f);

        renderer.PollEvents();

        mat.SetInUse();
        tex.Use(0);
        mat.SetUniformPVM(projection, view, model1);

        renderer.DirectRender(Transform(model1), spr1.GetMesh(), mat);

        mat.SetUniformPVM(projection, view, model2);
        renderer.DirectRender(Transform(model2), spr2.GetMesh(), mat);

        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}