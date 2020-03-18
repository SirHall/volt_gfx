#pragma once
#ifndef ShadeletSource_hpp
#define ShadeletSource_hpp

#include <string>

#include "volt/gfx/GLImport.hpp"

namespace volt::gfx
{
    struct ShadeletType
    {
        static constexpr GLenum Vertex   = GL_VERTEX_SHADER;
        static constexpr GLenum Fragment = GL_FRAGMENT_SHADER;
        static constexpr GLenum Geometry = GL_GEOMETRY_SHADER;
    };

    class ShadeletSource
    {
    private:
        std::string src;
        GLenum      tp;
        std::string path;

    public:
        ShadeletSource(std::string path, GLenum tp);
        ShadeletSource(const ShadeletSource &other);
        ShadeletSource &operator=(const ShadeletSource &other);
        ShadeletSource(ShadeletSource &&other);
        ShadeletSource &operator=(ShadeletSource &&other);

        ~ShadeletSource();

        const std::string &GetFullPath() const;
        const std::string &GetSource() const;
        GLenum             GetType() const;

        bool LoadFromFile(std::string filePath, GLenum type);
    };
} // namespace volt::gfx

#endif
