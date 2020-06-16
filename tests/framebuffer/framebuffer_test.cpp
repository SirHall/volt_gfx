#include "volt/event.hpp"
#include "volt/gfx.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include <boost/filesystem.hpp>
#include <filesystem>

using namespace volt::gfx;

float  mouseXDiff = 0.0f, mouseYDiff = 0.0f;
double lastMouseX = 0.0, lastMouseY = 0.0;
float  scale = 1.0f, scaleDeltaSpeed = 0.01f;
bool   firstUpdate = true;

bool liveMode = true;

int main(int argc, char *argv[])
{
    std::cout << "WD: " << std::filesystem::current_path() << std::endl;

    // The current frame number
    std::size_t frame = 0;

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

    renderer.Fullscreen();

    std::function<void(void)> screenshotFunc;

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
            if (e.GetKey() == GLFW_KEY_F)
                screenshotFunc();
        });

    // Resize the context when the frame buffer gets resized
    auto frameBufferResizeObserver =
        volt::event::observer<GFXEventFramebufferSize>(
            [&](GFXEventFramebufferSize &e) { renderer.CorrectContextSize(); });

    auto cursorPosObserver =
        volt::event::observer<GFXEventCursorPos>([&](GFXEventCursorPos &e) {
            mouseXDiff = (float)lastMouseX - (float)e.GetXPos();
            mouseYDiff = (float)lastMouseY - (float)e.GetYPos();

            lastMouseX = e.GetXPos();
            lastMouseY = e.GetYPos();
        });

#pragma endregion

#pragma region Compile Shaders

    // Compile Shader 1
    auto           shaderSource = ShaderSource("fb");
    ShadeletSource shadeletVert =
        ShadeletSource("res/shader_vert.glsl", ShadeletType::Vertex);
    ShadeletSource shadeletFrag =
        ShadeletSource("res/fb_raymarch_frag.glsl", ShadeletType::Fragment);
    // ShadeletSource("res/fb_mandelbrot_frag.glsl", ShadeletType::Fragment);
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

    // Compile shader 2
    auto           shaderSource2 = ShaderSource("default");
    ShadeletSource shadeletVert2 =
        ShadeletSource("res/shader_vert.glsl", ShadeletType::Vertex);

    ShadeletSource shadeletFrag2 =
        ShadeletSource("res/shader_frag.glsl", ShadeletType::Fragment);

    shaderSource2.AddShadelet(shadeletVert2);
    shaderSource2.AddShadelet(shadeletFrag2);

    auto shaderCompileErrors2 = std::vector<std::string>();
    auto shader2 = Shader::CompileShader(shaderSource2, shaderCompileErrors2);

    std::cout << shader2.GetProgram() << std::endl;

    if (shaderCompileErrors2.size() > 0)
    {
        for (auto error : shaderCompileErrors2)
            std::cerr << error << std::endl;
        exit(1);
    }
    else if (!shader2.IsValid())
    {
        std::cerr << "Shader2 is invalid" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "All shaders compiled successfully" << std::endl;
    }

#pragma endregion

    Material mat  = Material(shader);
    Material mat2 = Material(shader2);

    Texture tex = Texture(1920, 1080, false);

    Texture infernoTex = Texture(Image("./res/inferno.png"), false);

    infernoTex.Use(1);
    mat.SetUniformTex(1);

    tex.Use(0);
    mat2.SetUniformTex(0);

    Framebuffer framebuffer = Framebuffer();
    framebuffer.AttachTexture(tex, FramebufferTarget::ReadWrite(), 0);

    RenderObject obj2 =
        RenderObject(mat2,
                     Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                        glm::vec2(1.0f, 1.0f)),
                     Transform(glm::translate(glm::mat4(1.0f),
                                              glm::vec3(0.0f, 0.0f, 0.0f))));

    Camera cam = Camera();
    cam.SetTransform(Transform(glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f))));
    cam.SetPerspectiveMode(false);
    cam.SetOrthoSize(1.0f);
    cam.SetNearFarPlanes(0.1f, 100.0f);

    screenshotFunc = [&]() {
        auto img = framebuffer.RetreiveImage();
        if (img)
            img->Save("img.png");
    };

    // Loop until the user closes the window
    while (renderer.WindowOpen())
    {
        float ratio = renderer.GetFrameBufferSizeRatio();

        // This is recalculated each frame because the window can be resized

        renderer.PollEvents();

        // Move cam around the location

        obj2.GetMesh() =
            Sprite::CreateMesh(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                               glm::vec2(1.0f * ratio, 1.0f), false);

        // obj2.GetTransform() = Transform(
        //     glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

        cam.SetAspectRatio(ratio);

        float dt = liveMode ? renderer.GetDeltaTime() : 1.0f / 30.0f;
        float t  = liveMode ? renderer.GetUpTime() : dt * frame;

        GLint uniformLoc = -1;
        mat.Bind();
        // Set time uniforms
        if (mat.GetShader().GetUniformLocation("dt", uniformLoc))
            mat.GetShader().SetUniform(uniformLoc, dt);
        if (mat.GetShader().GetUniformLocation("t", uniformLoc))
            mat.GetShader().SetUniform(uniformLoc, t);
        if (mat.GetShader().GetUniformLocation("ratio", uniformLoc))
            mat.GetShader().SetUniform(uniformLoc, ratio);
        if (mat.GetShader().GetUniformLocation("camPos", uniformLoc))
            mat.GetShader().SetUniform(uniformLoc,
                                       cam.GetTransform().GetPosition());

        // Firstly render to the texture
        renderer.RenderFramebuffer(framebuffer, mat);
        tex.GenerateMipmap();

        Framebuffer::BindDefaultFramebuffer();
        renderer.DirectRender(obj2, cam);

        renderer.DisplayFrame();

        // If live, render in realtime, otherwise render to a file
        if (liveMode)
        {
            renderer.SleepForFrame();
            std::cout << 1.0f / renderer.GetDeltaTime() << std::endl;
        }
        else
        {
            auto fileName = std::stringstream();
            fileName << "frame_" << frame << ".png";
            auto img = framebuffer.RetreiveImage();
            if (img)
                img->Save(fileName.str());
        }
        frame++;
    }
    return 0;
}
