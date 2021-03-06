﻿#include "volt/gfx/VAOGen.hpp"
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
    GLCall(glVertexAttribDivisor(startIndex, instanced ? 1 : 0));
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
    GLCall(glVertexAttribDivisor(startIndex, instanced ? 1 : 0));
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
            startIndex, GLint(J), GetGlType<float>(), GL_FALSE, GLsizei(stride),
            reinterpret_cast<void const *>(offset + (i * J * sizeof(float)))));
        GLCall(glVertexAttribDivisor(startIndex, instanced ? 1 : 0));
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

template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::float32>()
{
    return GLsizei(1);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::vec1>()
{
    return GLsizei(1);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::vec2>()
{
    return GLsizei(1);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::vec3>()
{
    return GLsizei(1);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::vec4>()
{
    return GLsizei(1);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat2x2>()
{
    return GLsizei(2);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat2x3>()
{
    return GLsizei(2);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat2x4>()
{
    return GLsizei(2);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat3x2>()
{
    return GLsizei(3);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat3x3>()
{
    return GLsizei(3);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat3x4>()
{
    return GLsizei(3);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat4x2>()
{
    return GLsizei(4);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat4x3>()
{
    return GLsizei(4);
}
template <>
GLsizei volt::gfx::VAOGen::AttribTypeSize<glm::mat4x4>()
{
    return GLsizei(4);
}
