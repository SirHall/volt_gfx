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

// Get OpenGL types intelligently and at compile time
template <typename T>
constexpr GLenum GetGlType();

template <>
constexpr GLenum GetGlType<float>();

template <>
constexpr GLenum GetGlType<int>();

template <>
constexpr GLenum GetGlType<unsigned int>();

template <>
constexpr GLenum GetGlType<char>();

template <>
constexpr GLenum GetGlType<unsigned char>();

#endif