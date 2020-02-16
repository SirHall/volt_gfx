#include "volt/event.hpp"
#include "volt/gfx.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

using namespace volt::gfx;

int main(int argc, char *argv[])
{
#pragma region Setup Renderer and Window

    auto renderer = Renderer();

    auto windowSettings   = WindowCreationDataSettings();
    windowSettings.height = 640;
    windowSettings.width  = 640;
    windowSettings.title  = "VoltGFX";
    if (!renderer.Initialize(windowSettings))
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

    // std::cout << "Reading resources from: "
    //           << execPath.generic_string() + "/res" << std::endl;

    auto shaderSources =
        ShaderSource::ReadShaderSources(execPath.generic_string() + "/res");
    auto shaderCompileErrors = std::make_unique<std::vector<std::string>>();
    auto shaders = Shader::CompileShaders(shaderSources, shaderCompileErrors);

    if (shaderCompileErrors->size() > 0)
    {
        for (auto error : *shaderCompileErrors)
            std::cout << error << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "All shaders compiled successfully" << std::endl;
    }

    std::cout << "Compiled " << shaders->size() << " shader(s)" << std::endl;

#pragma endregion

    Transform transform = Transform(glm::mat4(1.0f));

    Sprite spr1 =
        Sprite(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(5.0f, 5.0f));
    Sprite spr2 =
        Sprite(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(5.0f, 5.0f));

    auto w4 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    std::cout << w4.x << " " << w4.y << " " << w4.z << " " << w4.w << std::endl;

    // Load uv sprite
    Texture tex = Texture::LoadFromFile("res/tree.png");
    tex.LoadIntoVRAM();

    glm::mat4 model =
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
        model = glm::rotate(model, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));

        // Projection matrix
        // glm::mat4 projection =
        //     glm::ortho(0.0f, (GLfloat)1, (GLfloat)1, 0.0f, 0.1f, 100.0f);
        auto [frameBufferWidth, frameBufferHeight] =
            renderer.GetFrameBufferSize();

        // This is recalculated each frame because the window can be resized
        glm::mat4 projection = glm::perspective(
            45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.1f,
            100.0f);

        renderer.PollEvents();

        tex.Use(0);
        Shader &shader = shaders->at(0);
        shader.SetInUse();

        GLint uniformLoc = -1;
        if (shader.GetUniformLocation("projection", uniformLoc))
            shader.SetUniform(uniformLoc, projection);

        if (shader.GetUniformLocation("view", uniformLoc))
            shader.SetUniform(uniformLoc, view);

        if (shader.GetUniformLocation("model", uniformLoc))
            shader.SetUniform(uniformLoc, model);

        if (shader.GetUniformLocation("tex", uniformLoc))
            shader.SetUniform(uniformLoc, 0);

        renderer.DirectRender(transform, spr1.GetMesh(), shaders->at(0));

        if (shader.GetUniformLocation("model", uniformLoc))
            shader.SetUniform(uniformLoc, model2);

        renderer.DirectRender(transform, spr2.GetMesh(), shaders->at(0));

        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}