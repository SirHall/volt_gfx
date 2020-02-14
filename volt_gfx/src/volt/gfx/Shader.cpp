#include "volt/gfx/Shader.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

using namespace volt::gfx;

std::unique_ptr<std::string>
    Shader::CompileShadelet(const ShadeletSource &shadeletSource, GLuint &id)
{
    id              = gl::CreateShader(shadeletSource.GetType());
    const char *src = shadeletSource.GetSource().c_str();
    gl::ShaderSource(id, 1, &src, nullptr);
    gl::CompileShader(id);

    GLint result = 0;
    gl::GetShaderiv(id, gl::COMPILE_STATUS, &result);
    if (result == gl::FALSE_)
    {
        GLint len = 0;
        gl::GetShaderiv(id, gl::INFO_LOG_LENGTH, &len);

        GLchar *msg = (GLchar *)malloc(len);
        gl::GetShaderInfoLog(id, len, &len, msg);

        auto return_msg = std::make_unique<std::string>(
            "Error in shader '" + shadeletSource.GetFullPath() + "':\n" +
            std::string((char *)msg));
        delete msg;
        return return_msg;
    }

    return std::unique_ptr<std::string>(nullptr);
}

std::unique_ptr<std::vector<Shader>>
    Shader::CompileShaders(const std::vector<ShaderSource> &          sources,
                           std::unique_ptr<std::vector<std::string>> &errors)
{

    // auto shaderIDs = std::vector<GLuint>();
    auto shaders = std::make_unique<std::vector<Shader>>();

    for (auto &source : sources)
    {
        bool        anyErrors  = false;
        GLuint      programID  = gl::CreateProgram();
        GLuint      shadeletID = 0;
        auto const &shadelets  = source.GetShadelets();
        // Compile and add each individual shadelet
        for (auto const &shadelet : shadelets)
        {
            auto compileResult = CompileShadelet(shadelet, shadeletID);
            if (compileResult == nullptr) // No error occurred
            {
                // shaderIDs.push_back(shadeletID);
                GLCall(gl::AttachShader(programID, shadeletID));
                GLCall(gl::DeleteShader(shadeletID));
            }
            else
            {
                if (errors != nullptr)
                    errors->push_back(*compileResult);
                anyErrors = true;
                return shaders;
            }
        }
        GLCall(gl::LinkProgram(programID));

        // Check for link-time errors
        GLint linkStatus = 0;
        GLCall(gl::GetProgramiv(programID, gl::LINK_STATUS, &linkStatus));
        if (linkStatus == gl::FALSE_)
        {
            // Linking failed
            GLint len = 0;
            GLCall(gl::GetProgramiv(programID, gl::INFO_LOG_LENGTH, &len));
            GLchar *msg = (GLchar *)malloc(len);

            GLCall(gl::GetProgramInfoLog(programID, len, &len, msg));

            std::string err = "Linker error in shader '" + source.GetName() +
                              "':\n" + std::string((char *)msg);

            errors->push_back(err);
            delete msg;
            anyErrors = true;
        }

        if (!anyErrors)
            shaders->push_back(Shader(programID));
        // gl::ValidateProgram(programID);
    }

    return shaders;
}

GLuint Shader::GetProgram() const { return this->program; }

void Shader::SetInUse() const { GLCall(gl::UseProgram(this->program)); }

bool Shader::GetUniformLocation(const std::string &uniformName,
                                GLint &            uniformLocOut) const
{
    GLCall(uniformLocOut =
               gl::GetUniformLocation(this->program, uniformName.c_str()));
    return uniformLocOut >= 0;
}

Shader::Shader(GLuint programID) : program(programID) {}

Shader::~Shader() {}

// template <typename T>
// void Shader::SetUniform(GLint uniformLoc, const T &value)
// {
//     AssignUniform<T>(uniformLoc, value);
// }

#pragma region Uniform Assignment

namespace volt::gfx
{
    template <typename T>
    void AssignUniform(GLint uniformLoc, const T &value)
    {
        std::cerr << "AssignUniform for this type has not been implemented"
                  << std::endl;
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLuint &value)
    {
        gl::Uniform1ui(uniformLoc, value);
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLint &value)
    {
        gl::Uniform1i(uniformLoc, value);
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLfloat &value)
    {
        gl::Uniform1f(uniformLoc, value);
    }

    // template <>
    // void AssignUniform(GLint uniformLoc, const glm::vec1 &value)
    // {
    //     gl::Uniform1fv(uniformLoc, 1, glm::value_ptr(value));
    // }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec2 &value)
    {
        gl::Uniform2fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec3 &value)
    {
        gl::Uniform3fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec4 &value)
    {
        gl::Uniform4fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec1> &value)
    {
        gl::Uniform1fv(uniformLoc, value.size(), (const float *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec2> &value)
    {
        gl::Uniform2fv(uniformLoc, value.size(), (const float *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec3> &value)
    {
        gl::Uniform3fv(uniformLoc, value.size(), (const float *)value.size());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec4> &value)
    {
        gl::Uniform4fv(uniformLoc, value.size(), (const float *)value.size());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::quat &value)
    {
        gl::Uniform4fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2 &value)
    {
        gl::UniformMatrix2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3 &value)
    {
        gl::UniformMatrix3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4 &value)
    {
        gl::UniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x3 &value)
    {
        gl::UniformMatrix2x3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x2 &value)
    {
        gl::UniformMatrix3x2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x4 &value)
    {
        gl::UniformMatrix2x4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x2 &value)
    {
        gl::UniformMatrix4x2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x4 &value)
    {
        gl::UniformMatrix3x4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x3 &value)
    {
        gl::UniformMatrix4x3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2> &value)
    {
        gl::UniformMatrix2fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3> &value)
    {
        gl::UniformMatrix3fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4> &value)
    {
        gl::UniformMatrix4fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x3> &value)
    {
        gl::UniformMatrix2x3fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x2> &value)
    {
        gl::UniformMatrix3x2fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x4> &value)
    {
        gl::UniformMatrix2x4fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x2> &value)
    {
        gl::UniformMatrix4x2fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x4> &value)
    {
        gl::UniformMatrix3x4fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x3> &value)
    {
        gl::UniformMatrix4x3fv(uniformLoc, value.size(), false,
                               (const GLfloat *)value.data());
    }
} // namespace volt::gfx

#pragma endregion