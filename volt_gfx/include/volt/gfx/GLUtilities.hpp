#pragma once
#ifndef OpenGLExtensions_hpp
#define OpenGLExtensions_hpp

#include "volt/gfx/GLImport.hpp"

void GLClearError();

void GLCheckError(const char *func, const char *file, int line);

void PrintGLError(GLenum errorCode);

// Only enable this call if we are in debug mode
#ifndef NDEBUG
// Debug mode
#define GLCall(x)                                                              \
    GLClearError();                                                            \
    x;                                                                         \
    GLCheckError(#x, __FILE__, __LINE__)
#else
// Release mode
#define GLCall(x) x
#endif
namespace volt::gfx
{
    // Get OpenGL types intelligently and at compile time
    template <typename T>
    GLenum GetGlType();

    template <>
    GLenum GetGlType<float>();

    template <>
    GLenum GetGlType<int>();

    template <>
    GLenum GetGlType<unsigned int>();

    template <>
    GLenum GetGlType<char>();

    template <>
    GLenum GetGlType<unsigned char>();
} // namespace volt::gfx
#endif