#include "volt/gfx/Shader.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

using namespace volt::gfx;

bool Shader::CompileShadelet(ShadeletSource const &shadeletSource, GLuint &id,
                             std::vector<std::string> &errors)
{
    GLCall(id = gl::CreateShader(shadeletSource.GetType()));
    const char *src = shadeletSource.GetSource().c_str();
    GLCall(gl::ShaderSource(id, 1, &src, nullptr));
    GLCall(gl::CompileShader(id));

    GLint result = 0;
    GLCall(gl::GetShaderiv(id, gl::COMPILE_STATUS, &result));
    if (result == gl::FALSE_)
    {
        GLint len = 0;
        GLCall(gl::GetShaderiv(id, gl::INFO_LOG_LENGTH, &len));

        GLchar *msg = (GLchar *)malloc(len);
        GLCall(gl::GetShaderInfoLog(id, len, &len, msg));

        errors.push_back(std::string("Error in shader '" +
                                     shadeletSource.GetFullPath() + "':\n" +
                                     std::string(static_cast<char *>(msg))));
        delete msg;
        return false;
    }

    return true;
}
Shader Shader::CompileShader(ShaderSource const &      source,
                             std::vector<std::string> &errors)
{
    GLCall(GLuint programID = gl::CreateProgram());
    GLuint shadeletID = 0;

    // Compile and add each individual shadelet
    for (auto const &shadelet : source.GetShadelets())
    {
        if (CompileShadelet(shadelet, shadeletID, errors)) // No error occurred
        {
            GLCall(gl::AttachShader(programID, shadeletID));
            GLCall(gl::DeleteShader(shadeletID));
        }
        else
        {
            GLCall(gl::DeleteProgram(programID));
            return Shader(0);
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

        errors.push_back(std::move(err));
        delete msg;
        return Shader(0);
    }

    // Linking succeeded
    return std::move(Shader(programID));
}

GLuint Shader::GetProgram() const { return *this->program; }

void Shader::SetInUse() const { GLCall(gl::UseProgram(*this->program)); }

bool Shader::GetUniformLocation(const std::string &uniformName,
                                GLint &            uniformLocOut) const
{
    GLCall(uniformLocOut =
               gl::GetUniformLocation(*this->program, uniformName.c_str()));
    return uniformLocOut >= 0;
}

Shader::Shader(GLuint programID)
    : program(std::move(
          std::shared_ptr<GLuint>(new GLuint(programID), [=](GLuint *ptr) {
              if (*ptr > 0)
                  GLCall(gl::DeleteProgram(*ptr));
              delete ptr;
          })))
{
}

Shader::Shader(const Shader &other) : program(other.program) {}

Shader &Shader::operator=(const Shader &other)
{
    this->program = other.program;
    return *this;
}

Shader::Shader(Shader &&other) : program(std::move(other.program)) {}

Shader &Shader::operator=(Shader &&other)
{
    this->program = std::move(other.program);
    return *this;
}

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
        GLCall(gl::Uniform1ui(uniformLoc, value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLint &value)
    {
        GLCall(gl::Uniform1i(uniformLoc, value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLfloat &value)
    {
        GLCall(gl::Uniform1f(uniformLoc, value));
    }

    // template <>
    // void AssignUniform(GLint uniformLoc, const glm::vec1 &value)
    // {
    //     gl::Uniform1fv(uniformLoc, 1, glm::value_ptr(value));
    // }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec2 &value)
    {
        GLCall(gl::Uniform2fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec3 &value)
    {
        GLCall(gl::Uniform3fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec4 &value)
    {
        GLCall(gl::Uniform4fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec1> &value)
    {
        GLCall(gl::Uniform1fv(uniformLoc, value.size(),
                              (const float *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec2> &value)
    {
        GLCall(gl::Uniform2fv(uniformLoc, value.size(),
                              (const float *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec3> &value)
    {
        GLCall(gl::Uniform3fv(uniformLoc, value.size(),
                              (const float *)value.size()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec4> &value)
    {
        GLCall(gl::Uniform4fv(uniformLoc, value.size(),
                              (const float *)value.size()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::quat &value)
    {
        GLCall(gl::Uniform4fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2 &value)
    {
        GLCall(
            gl::UniformMatrix2fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3 &value)
    {
        GLCall(
            gl::UniformMatrix3fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4 &value)
    {
        GLCall(
            gl::UniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x3 &value)
    {
        GLCall(gl::UniformMatrix2x3fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x2 &value)
    {
        GLCall(gl::UniformMatrix3x2fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x4 &value)
    {
        GLCall(gl::UniformMatrix2x4fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x2 &value)
    {
        GLCall(gl::UniformMatrix4x2fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x4 &value)
    {
        GLCall(gl::UniformMatrix3x4fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x3 &value)
    {
        GLCall(gl::UniformMatrix4x3fv(uniformLoc, 1, false,
                                      glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2> &value)
    {
        GLCall(gl::UniformMatrix2fv(uniformLoc, value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3> &value)
    {
        GLCall(gl::UniformMatrix3fv(uniformLoc, value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4> &value)
    {
        GLCall(gl::UniformMatrix4fv(uniformLoc, value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x3> &value)
    {
        GLCall(gl::UniformMatrix2x3fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x2> &value)
    {
        GLCall(gl::UniformMatrix3x2fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x4> &value)
    {
        GLCall(gl::UniformMatrix2x4fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x2> &value)
    {
        GLCall(gl::UniformMatrix4x2fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x4> &value)
    {
        GLCall(gl::UniformMatrix3x4fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x3> &value)
    {
        GLCall(gl::UniformMatrix4x3fv(uniformLoc, value.size(), false,
                                      (const GLfloat *)value.data()));
    }
} // namespace volt::gfx

#pragma endregion