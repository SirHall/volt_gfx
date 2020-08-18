#pragma once
#ifndef VOLT_GFX_VAOGEN_HPP
#define VOLT_GFX_VAOGEN_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <glm/glm.hpp>

#include <string>

namespace volt::gfx::VAOGen
{

    GLuint GenerateVAO();

    // Returns next vertex attribute index
    template <typename T>
    GLuint GenVertAttrib(std::string const &name, GLuint startIndex,
                         std::size_t stride, std::size_t offset,
                         bool instanced = false);
    // { // TODO: Ensure that this can only run with integral types
    //     GLCall(glEnableVertexAttribArray(startIndex));
    //     GLCall(glVertexAttribPointer(startIndex, 1, GetGlType<T>(), GL_FALSE,
    //                                  stride, reinterpret_cast<void
    //                                  *>(offset)));
    //     if (instanced)
    //         glVertexAttribDivisor(startIndex, 1);
    //     return startIndex + 1;
    // }

    // Declare vertex attribute generators for specialized types
    // // Integers
    // template <>
    // GLuint GenVertAttrib<glm::int8>(std::string const &name, GLuint
    // startIndex,
    //                                 std::size_t stride, std::size_t offset,
    //                                 bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::int16>(std::string const &name, GLuint
    // startIndex,
    //                                  std::size_t stride, std::size_t offset,
    //                                  bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::int32>(std::string const &name, GLuint
    // startIndex,
    //                                  std::size_t stride, std::size_t offset,
    //                                  bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::int64>(std::string const &name, GLuint
    // startIndex,
    //                                  std::size_t stride, std::size_t offset,
    //                                  bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::uint8>(std::string const &name, GLuint
    // startIndex,
    //                                  std::size_t stride, std::size_t offset,
    //                                  bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::uint16>(std::string const &name,
    //                                   GLuint startIndex, std::size_t stride,
    //                                   std::size_t offset, bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::uint32>(std::string const &name,
    //                                   GLuint startIndex, std::size_t stride,
    //                                   std::size_t offset, bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::uint64>(std::string const &name,
    //                                   GLuint startIndex, std::size_t stride,
    //                                   std::size_t offset, bool instanced);

    // // Floating point
    template <>
    GLuint GenVertAttrib<glm::float32>(std::string const &name,
                                       GLuint startIndex, std::size_t stride,
                                       std::size_t offset, bool instanced);
    // template <>
    // GLuint GenVertAttrib<glm::float64>(std::string const &name,
    //                                    GLuint startIndex, std::size_t stride,
    //                                    std::size_t offset, bool instanced);

    // Vectors
    template <>
    GLuint GenVertAttrib<glm::vec1>(std::string const &name, GLuint startIndex,
                                    std::size_t stride, std::size_t offset,
                                    bool instanced);
    template <>
    GLuint GenVertAttrib<glm::vec2>(std::string const &name, GLuint startIndex,
                                    std::size_t stride, std::size_t offset,
                                    bool instanced);
    template <>
    GLuint GenVertAttrib<glm::vec3>(std::string const &name, GLuint startIndex,
                                    std::size_t stride, std::size_t offset,
                                    bool instanced);
    template <>
    GLuint GenVertAttrib<glm::vec4>(std::string const &name, GLuint startIndex,
                                    std::size_t stride, std::size_t offset,
                                    bool instanced);

    // Matrices
    template <>
    GLuint GenVertAttrib<glm::mat2x2>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat2x3>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat2x4>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat3x2>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat3x3>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat3x4>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat4x2>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat4x3>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);
    template <>
    GLuint GenVertAttrib<glm::mat4x4>(std::string const &name,
                                      GLuint startIndex, std::size_t stride,
                                      std::size_t offset, bool instanced);

    template <typename T>
    GLsizei AttribTypeSize();

    template <>
    GLsizei AttribTypeSize<glm::float32>();
    template <>
    GLsizei AttribTypeSize<glm::vec1>();
    template <>
    GLsizei AttribTypeSize<glm::vec2>();
    template <>
    GLsizei AttribTypeSize<glm::vec3>();
    template <>
    GLsizei AttribTypeSize<glm::vec4>();
    template <>
    GLsizei AttribTypeSize<glm::mat2x2>();
    template <>
    GLsizei AttribTypeSize<glm::mat2x3>();
    template <>
    GLsizei AttribTypeSize<glm::mat2x4>();
    template <>
    GLsizei AttribTypeSize<glm::mat3x2>();
    template <>
    GLsizei AttribTypeSize<glm::mat3x3>();
    template <>
    GLsizei AttribTypeSize<glm::mat4x4>();
    template <>
    GLsizei AttribTypeSize<glm::mat4x2>();
    template <>
    GLsizei AttribTypeSize<glm::mat4x3>();
    template <>
    GLsizei AttribTypeSize<glm::mat4x4>();

} // namespace volt::gfx::VAOGen
#endif
