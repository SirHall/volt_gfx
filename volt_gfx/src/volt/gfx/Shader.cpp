#include "volt/gfx/Shader.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "volt/gfx/GLUtilities.hpp"

// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End
using namespace volt::gfx;

std::unique_ptr<std::string>
    Shader::CompileShadelet(const ShadeletSource &shadeletSource, GLuint &id)
{
    id              = glCreateShader(shadeletSource.GetType());
    const char *src = shadeletSource.GetSource().c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint len = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        GLchar *msg = (GLchar *)malloc(len);
        glGetShaderInfoLog(id, len, &len, msg);

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
        GLuint      programID  = glCreateProgram();
        GLuint      shadeletID = 0;
        auto const &shadelets  = source.GetShadelets();
        // Compile and add each individual shadelet
        for (auto const &shadelet : shadelets)
        {
            auto compileResult = CompileShadelet(shadelet, shadeletID);
            if (compileResult == nullptr) // No error occurred
            {
                // shaderIDs.push_back(shadeletID);
                glAttachShader(programID, shadeletID);
                glDeleteShader(shadeletID);
            }
            else
            {
                if (errors != nullptr)
                    errors->push_back(*compileResult);
                anyErrors = true;
                return shaders;
            }
        }
        glLinkProgram(programID);

        // Check for link-time errors
        GLint linkStatus = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            // Linking failed
            GLint len = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);
            GLchar *msg = (GLchar *)malloc(len);

            glGetProgramInfoLog(programID, len, &len, msg);

            std::string err = "Linker error in shader '" + source.GetName() +
                              "':\n" + std::string((char *)msg);

            errors->push_back(err);
            delete msg;
            anyErrors = true;
        }

        if (!anyErrors)
            shaders->push_back(Shader(programID));
        // glValidateProgram(programID);
    }

    return shaders;
}

GLuint Shader::GetProgram() const { return this->program; }

void Shader::SetInUse() const { GLCall(glUseProgram(this->program)); }

bool Shader::GetUniformLocation(const std::string &uniformName,
                                GLint &            uniformLocOut) const
{
    GLCall(uniformLocOut =
               glGetUniformLocation(this->program, uniformName.c_str()));
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
        glUniform1ui(uniformLoc, value);
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLint &value)
    {
        glUniform1i(uniformLoc, value);
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLfloat &value)
    {
        glUniform1f(uniformLoc, value);
    }

    // template <>
    // void AssignUniform(GLint uniformLoc, const glm::vec1 &value)
    // {
    //     glUniform1fv(uniformLoc, 1, glm::value_ptr(value));
    // }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec2 &value)
    {
        glUniform2fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec3 &value)
    {
        glUniform3fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec4 &value)
    {
        glUniform4fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec1> &value)
    {
        glUniform1fv(uniformLoc, value.size(), (const float *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec2> &value)
    {
        glUniform2fv(uniformLoc, value.size(), (const float *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec3> &value)
    {
        glUniform3fv(uniformLoc, value.size(), (const float *)value.size());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec4> &value)
    {
        glUniform4fv(uniformLoc, value.size(), (const float *)value.size());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::quat &value)
    {
        glUniform4fv(uniformLoc, 1, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2 &value)
    {
        glUniformMatrix2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3 &value)
    {
        glUniformMatrix3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4 &value)
    {
        glUniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x3 &value)
    {
        glUniformMatrix2x3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x2 &value)
    {
        glUniformMatrix3x2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x4 &value)
    {
        glUniformMatrix2x4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x2 &value)
    {
        glUniformMatrix4x2fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x4 &value)
    {
        glUniformMatrix3x4fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x3 &value)
    {
        glUniformMatrix4x3fv(uniformLoc, 1, false, glm::value_ptr(value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2> &value)
    {
        glUniformMatrix2fv(uniformLoc, value.size(), false,
                           (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3> &value)
    {
        glUniformMatrix3fv(uniformLoc, value.size(), false,
                           (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4> &value)
    {
        glUniformMatrix4fv(uniformLoc, value.size(), false,
                           (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x3> &value)
    {
        glUniformMatrix2x3fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x2> &value)
    {
        glUniformMatrix3x2fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x4> &value)
    {
        glUniformMatrix2x4fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x2> &value)
    {
        glUniformMatrix4x2fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x4> &value)
    {
        glUniformMatrix3x4fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x3> &value)
    {
        glUniformMatrix4x3fv(uniformLoc, value.size(), false,
                             (const GLfloat *)value.data());
    }
} // namespace volt::gfx

#pragma endregion