#include "volt/gfx/VAOGen.hpp"
#include "volt/gfx/GLUtilities.hpp"

using namespace volt::gfx::VAOGen;
using namespace volt::gfx;

GLuint volt::gfx::VAOGen::GenerateVAO()
{
    GLuint vao = 0;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    return vao;
}

template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::float32>(std::string const &name,
                                                      GLuint      startIndex,
                                                      std::size_t stride,
                                                      std::size_t offset,
                                                      bool        instanced)
{
    GLCall(glEnableVertexAttribArray(startIndex));
    GLCall(glVertexAttribPointer(startIndex, 1, GetGlType<float>(), GL_FALSE,
                                 GLsizei(stride),
                                 reinterpret_cast<void *>(offset)));
    if (instanced)
        glVertexAttribDivisor(startIndex, 1);
    return startIndex + 1;
}

GLuint GenNVecVertAttrib(std::string const &name, GLuint startIndex,
                         std::size_t stride, std::size_t offset, bool instanced,
                         std::size_t size)
{
    GLCall(glEnableVertexAttribArray(startIndex));
    GLCall(glVertexAttribPointer(startIndex, GLint(size), GetGlType<float>(),
                                 GL_FALSE, GLsizei(stride),
                                 reinterpret_cast<GLvoid *>(offset)));
    if (instanced)
        GLCall(glVertexAttribDivisor(startIndex, 1));
    return startIndex + 1;
}

template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::vec1>(std::string const &name,
                                                   GLuint      startIndex,
                                                   std::size_t stride,
                                                   std::size_t offset,
                                                   bool        instanced)
{
    return GenNVecVertAttrib(name, startIndex, stride, offset, instanced, 1);
}

template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::vec2>(std::string const &name,
                                                   GLuint      startIndex,
                                                   std::size_t stride,
                                                   std::size_t offset,
                                                   bool        instanced)
{
    return GenNVecVertAttrib(name, startIndex, stride, offset, instanced, 2);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::vec3>(std::string const &name,
                                                   GLuint      startIndex,
                                                   std::size_t stride,
                                                   std::size_t offset,
                                                   bool        instanced)
{
    return GenNVecVertAttrib(name, startIndex, stride, offset, instanced, 3);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::vec4>(std::string const &name,
                                                   GLuint      startIndex,
                                                   std::size_t stride,
                                                   std::size_t offset,
                                                   bool        instanced)
{
    return GenNVecVertAttrib(name, startIndex, stride, offset, instanced, 4);
}

GLuint GemNMatVertAttrib(std::string const &name, GLuint startIndex,
                         std::size_t stride, std::size_t offset, bool instanced,
                         std::size_t I, std::size_t J)
{
    for (std::size_t i = 0; i < I; i++)
    {
        GLCall(glEnableVertexAttribArray(startIndex));
        GLCall(glVertexAttribPointer(
            startIndex, J, GetGlType<float>(), GL_FALSE, stride,
            reinterpret_cast<void *>(offset + (i * J))));
        if (instanced)
            glVertexAttribDivisor(startIndex, 1);
        startIndex++;
    }
    return startIndex;
}

template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat2x2>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 2, 2);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat2x3>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 2, 3);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat2x4>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 2, 4);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat3x2>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 3, 2);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat3x3>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 3, 3);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat3x4>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 3, 4);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat4x2>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 4, 2);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat4x3>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 4, 3);
}
template <>
GLuint volt::gfx::VAOGen::GenVertAttrib<glm::mat4x4>(std::string const &name,
                                                     GLuint      startIndex,
                                                     std::size_t stride,
                                                     std::size_t offset,
                                                     bool        instanced)
{
    return GemNMatVertAttrib(name, startIndex, stride, offset, instanced, 4, 4);
}
