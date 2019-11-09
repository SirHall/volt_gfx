// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End

#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Renderer.hpp"
#include "volt/gfx/Shader.hpp"
#include "volt/gfx/Vertex.hpp"

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
        exit(1);
    }
    renderer.SetTargetFPS(60.0f);

#pragma endregion

#pragma region Compile Shaders

    // Get executable file path
    auto execPath = boost::filesystem::system_complete(argv[0]).parent_path();

    std::cout << "Reading resources from: "
              << execPath.generic_string() + "/res" << std::endl;

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

    std::cout << "Compiled " << shaders->size() << " shaders" << std::endl;

#pragma endregion

    Transform transform = Transform(glm::mat4(1.0f));

    Mesh mesh = Mesh();
    mesh.CreateMesh({Vertex(1.f, 1.f, 1.f, 0.0f, 0.0f),    // 0
                     Vertex(-1.f, 1.f, 1.f, 0.0f, 0.0f),   // 1
                     Vertex(-1.f, 1.f, -1.f, 0.0f, 0.0f),  // 2
                     Vertex(1.f, 1.f, -1.f, 0.0f, 0.0f),   // 3
                     Vertex(1.f, -1.f, 1.f, 0.0f, 0.0f),   // 4
                     Vertex(-1.f, -1.f, 1.f, 0.0f, 0.0f),  // 5
                     Vertex(-1.f, -1.f, -1.f, 0.0f, 0.0f), // 6
                     Vertex(1.f, -1.f, -1.f, 0.0f, 0.0f)}, // 7
                    {0, 1, 3, 3, 1, 2, 2, 6, 7, 7, 3, 2, 7, 6, 5, 5, 4, 7,
                     5, 1, 4, 4, 1, 0, 4, 3, 7, 3, 4, 0, 5, 6, 2, 5, 1, 2});
    // Projection matrix
    // glm::mat4 projection =
    //     glm::ortho(0.0f, (GLfloat)1, (GLfloat)1, 0.0f, 0.1f, 100.0f);
    glm::mat4 projection =
        glm::perspective(45.0f,
                         (GLfloat)renderer.GetBufferWidth() /
                             (GLfloat)renderer.GetBufferHeight(),
                         0.1f, 100.0f);

    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

    // Loop until the user closes the window
    while (renderer.WindowOpen())
    {
        renderer.PollEvents();

        Shader &shader = shaders->at(0);

        GLint uniformLoc = -1;
        if (shader.GetUniformLocation("projection", uniformLoc))
            shader.SetUniform(uniformLoc, projection);

        if (shader.GetUniformLocation("view", uniformLoc))
            shader.SetUniform(uniformLoc, view);

        if (shader.GetUniformLocation("model", uniformLoc))
            shader.SetUniform(uniformLoc, model);

        renderer.DirectRender(transform, mesh, shaders->at(0));
        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}