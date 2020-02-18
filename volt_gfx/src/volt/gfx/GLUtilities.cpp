#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/GLImport.hpp"

#include <cassert>
#include <iostream>

void GLClearError()
{
    // while (gl::GetError() != gl::NO_ERROR_) {}
}

void GLCheckError(const char *func, const char *file, int line)
{
    bool hadError = false;
    while (GLenum err = gl::GetError())
    {
        std::cerr << "[OpenGL Error] > 0x" << std::hex << err << std::dec
                  << "\n\tIn: " << file << ":" << line << "\n\t" << func
                  << std::endl;
        PrintGLError(err);
        hadError = true;
    }
    if (hadError)
    {
        std::exit(1);
    }
}

void PrintGLError(GLenum errorCode)
{
    // As defined by:
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetError.xhtml
    switch (errorCode)
    {
        case gl::NO_ERROR_:
            std::cerr << "[GL_NO_ERROR] No error has been recorded. The value "
                         "of this "
                         "symbolic constant is guaranteed to be 0."
                      << std::endl;
            break;
        case gl::INVALID_ENUM:
            std::cerr << "[GL_INVALID_ENUM] An unacceptable value is specified "
                         "for an enumerated "
                         "argument. The offending command is ignored and has "
                         "no other side effect than to set the error flag."
                      << std::endl;
            break;
        case gl::INVALID_VALUE:
            std::cerr << "[GL_INVALID_VALUE] A numeric argument is out of "
                         "range. The offending "
                         "command is ignored and has no other side effect than "
                         "to set the error flag."
                      << std::endl;
            break;
        case gl::INVALID_OPERATION:
            std::cerr << "[GL_INVALID_OPERATION] The specified operation is "
                         "not allowed in the "
                         "current state. The offending command is ignored and "
                         "has no other side effect than to set the error flag."
                      << std::endl;
            break;
        case gl::INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "[GL_INVALID_FRAMEBUFFER_OPERATION] The framebuffer "
                         "object is not complete. The "
                         "offending command is ignored and has no other side "
                         "effect than to set the error flag."
                      << std::endl;
            break;
        case gl::OUT_OF_MEMORY:
            std::cerr
                << "[GL_OUT_OF_MEMORY] There is not enough memory left to "
                   "execute the command. "
                   "The state of the GL is undefined, except for the state of "
                   "the error flags, after this error is recorded."
                << std::endl;
            break;
        // case gl::STACK_UNDERFLOW:
        //     std::cerr << "[GL_STACK_UNDERFLOW] An attempt has been made to "
        //                  "perform an operation "
        //                  "that would cause an internal stack to underflow."
        //               << std::endl;
        //     break;
        // case gl::STACK_OVERFLOW:
        //     std::cerr << "[GL_STACK_OVERFLOW] An attempt has been made to "
        //                  "perform an operation "
        //                  "that would cause an internal stack to overflow."
        //               << std::endl;
        //     break;
        default:
            std::cerr << "Not a recognized error code" << std::endl;
            break;
    }
}

// Get OpenGL types intelligently and at compile time
// template <typename T>
// constexpr GLenum GetGlType();

template <>
constexpr GLenum GetGlType<float>()
{
    return gl::FLOAT;
}

template <>
constexpr GLenum GetGlType<int>()
{
    return gl::INT;
}

template <>
constexpr GLenum GetGlType<unsigned int>()
{
    return gl::UNSIGNED_INT;
}

template <>
constexpr GLenum GetGlType<char>()
{
    return gl::BYTE;
}

template <>
constexpr GLenum GetGlType<unsigned char>()
{
    return gl::UNSIGNED_BYTE;
}
