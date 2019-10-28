#pragma once
#ifndef ShadeletSource_hpp
#define ShadeletSource_hpp

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace volt::gfx
{
    class ShadeletSource
    {
    private:
        std::string source;
        GLenum      type;
        std::string fullPath;

    public:
        ShadeletSource(std::string source, GLenum type, std::string fullPath);
        ~ShadeletSource();

        const std::string &GetFullPath() const;
        const std::string &GetSource() const;
        GLenum             GetType() const;
    };
} // namespace volt::gfx

#endif
