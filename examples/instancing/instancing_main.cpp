﻿#include "volt/event.hpp"
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
        ShadeletSource("res/instanced_vert.glsl", ShadeletType::Vertex);
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

    shader.SetVBODataTypes<MeshVertex, MeshInstanceData>();
    Material mat = Material(shader);

#pragma endregion

    // Load uv sprite
    Image   img = Image("res/tree.png");
    Texture tex = Texture(img);

    RenderObject obj1 =
        RenderObject(mat, Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                             glm::vec2(5.0f, 5.0f)));
    obj1.SetTexture(tex, 0);

    Camera cam = Camera();
    cam.SetPerspectiveMode(true);
    cam.SetPerspectiveFOV(45.0f);
    cam.SetNearFarPlanes(0.1f, 100.0f);
    cam.SetTransform(Transform(glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f))));

    auto instanceData = std::vector<MeshInstanceData>();
    instanceData.reserve(64);
    for (std::size_t i = 0; i < 8; i++)
        for (std::size_t j = 0; j < 8; j++)
            instanceData.push_back(MeshInstanceData(
                glm::scale(glm::translate(glm::mat4(1.0f),
                                          glm::vec3(float(i) - 4.0f,
                                                    float(j) - 4.0f, 0.0f)),
                           glm::vec3(0.1f))));

    Mesh  spriteMesh = Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                         glm::vec2(5.0f, 5.0f));
    auto &vao        = spriteMesh.GetVAO();

    vao.GetInstVBO().SetData(instanceData);

    if (!vao.IsValid())
    {
        std::cerr << "VAO is invalid" << std::endl;
        std::exit(1);
    }

    // Loop until the user closes
    // the window
    while (renderer.WindowOpen())
    {
        renderer.PollEvents();
        cam.SetAspectRatio(renderer.GetFrameBufferSizeRatio());

        // renderer.InstancedRender(obj1, instanceData, cam);
        // obj1.Bind();
        renderer.InstancedRender(vao, mat, cam);

        renderer.DisplayFrame();
        renderer.SleepForFrame();
    }

    return 0;
}