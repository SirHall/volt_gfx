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
    GLCall(id = glCreateShader(shadeletSource.GetType()));
    const char *src = shadeletSource.GetSource().c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    GLint result = 0;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        GLint len = 0;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));

        GLchar *msg = (GLchar *)malloc(len);
        GLCall(glGetShaderInfoLog(id, len, &len, msg));

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
    GLCall(GLuint programID = glCreateProgram());
    GLuint shadeletID = 0;

    // Compile and add each individual shadelet
    for (auto const &shadelet : source.GetShadelets())
    {
        if (CompileShadelet(shadelet, shadeletID, errors)) // No error occurred
        {
            GLCall(glAttachShader(programID, shadeletID));
            GLCall(glDeleteShader(shadeletID));
        }
        else
        {
            GLCall(glDeleteProgram(programID));
            return Shader(0);
        }
    }
    GLCall(glLinkProgram(programID));

    // Check for link-time errors
    GLint linkStatus = 0;
    GLCall(glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus));
    if (linkStatus == GL_FALSE)
    {
        // Linking failed
        GLint len = 0;
        GLCall(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len));
        GLchar *msg = (GLchar *)malloc(len);

        GLCall(glGetProgramInfoLog(programID, len, &len, msg));

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

void Shader::Bind() const { GLCall(glUseProgram(*this->program)); }

bool Shader::GetUniformLocation(const std::string &uniformName,
                                GLint &            uniformLocOut) const
{
    GLCall(uniformLocOut =
               glGetUniformLocation(*this->program, uniformName.c_str()));
    return uniformLocOut >= 0;
}

Shader::Shader(GLuint programID)
    : program(std::move(
          std::shared_ptr<GLuint>(new GLuint(programID), [=](GLuint *ptr) {
              if (*ptr > 0)
                  GLCall(glDeleteProgram(*ptr));
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
        GLCall(glUniform1ui(uniformLoc, value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLint &value)
    {
        GLCall(glUniform1i(uniformLoc, value));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const GLfloat &value)
    {
        GLCall(glUniform1f(uniformLoc, value));
    }

    // template <>
    // void AssignUniform(GLint uniformLoc, const glm::vec1 &value)
    // {
    //     glUniform1fv(uniformLoc, 1, glm::value_ptr(value));
    // }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec2 &value)
    {
        GLCall(glUniform2fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec3 &value)
    {
        GLCall(glUniform3fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec4 &value)
    {
        GLCall(glUniform4fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec1> &value)
    {
        GLCall(glUniform1fv(uniformLoc, (GLsizei)value.size(),
                            (const float *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec2> &value)
    {
        GLCall(glUniform2fv(uniformLoc, (GLsizei)value.size(),
                            (const float *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec3> &value)
    {
        GLCall(glUniform3fv(uniformLoc, (GLsizei)value.size(),
                            (const float *)value.size()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec4> &value)
    {
        GLCall(glUniform4fv(uniformLoc, (GLsizei)value.size(),
                            (const float *)value.size()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::quat &value)
    {
        GLCall(glUniform4fv(uniformLoc, 1, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2 &value)
    {
        GLCall(glUniformMatrix2fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3 &value)
    {
        GLCall(glUniformMatrix3fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4 &value)
    {
        GLCall(glUniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x3 &value)
    {
        GLCall(
            glUniformMatrix2x3fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x2 &value)
    {
        GLCall(
            glUniformMatrix3x2fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x4 &value)
    {
        GLCall(
            glUniformMatrix2x4fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x2 &value)
    {
        GLCall(
            glUniformMatrix4x2fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x4 &value)
    {
        GLCall(
            glUniformMatrix3x4fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x3 &value)
    {
        GLCall(
            glUniformMatrix4x3fv(uniformLoc, 1, false, glm::value_ptr(value)));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2> &value)
    {
        GLCall(glUniformMatrix2fv(uniformLoc, (GLsizei)value.size(), false,
                                  (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3> &value)
    {
        GLCall(glUniformMatrix3fv(uniformLoc, (GLsizei)value.size(), false,
                                  (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4> &value)
    {
        GLCall(glUniformMatrix4fv(uniformLoc, (GLsizei)value.size(), false,
                                  (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x3> &value)
    {
        GLCall(glUniformMatrix2x3fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x2> &value)
    {
        GLCall(glUniformMatrix3x2fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x4> &value)
    {
        GLCall(glUniformMatrix2x4fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x2> &value)
    {
        GLCall(glUniformMatrix4x2fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x4> &value)
    {
        GLCall(glUniformMatrix3x4fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x3> &value)
    {
        GLCall(glUniformMatrix4x3fv(uniformLoc, (GLsizei)value.size(), false,
                                    (const GLfloat *)value.data()));
    }
} // namespace volt::gfx

#pragma endregion