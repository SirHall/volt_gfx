#pragma once
#ifndef VOLT_GFX_TEXTURE_HPP
#define VOLT_GFX_TEXTURE_HPP

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <vector>

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Image.hpp"

#include <memory>

namespace volt::gfx::internal
{
    struct TexData
    {
    public:
        GLuint const texID;
        bool const   mipmaps;
        TexData(GLuint textureID, bool enableMipmaps);
    };
} // namespace volt::gfx::internal

namespace volt::gfx
{
    using namespace volt::gfx::internal;
    class Texture
    {
    private:
        std::shared_ptr<TexData> texData;

        void CreateTexture(GLsizei width, GLsizei height, bool enableMipmaps,
                           void const *data);

    public:
        Texture(Image const &image, bool enableMipmaps = true);
        Texture(GLsizei width, GLsizei height, bool enableMipmaps = true);

        Texture(Texture const &other);
        Texture(Texture &&other);
        Texture &operator=(const Texture &other);
        Texture &operator=(Texture &&other);
        ~Texture();

        void Use(unsigned int unitIndex);

        void Bind();
        void Unbind();

        GLuint GetTexID();

        void GenerateMipmap();

        GLsizei GetWidth();
        GLsizei GetHeight();

        void SetImage(Image const &image);
        void SetImage(void const *data, GLsizei width, GLsizei height);

        // Wil resize a texture on th GPU, useful for framebuffers when the
        // window is resized
        void Resize(GLsizei newWidth, GLsizei newHeight);
    };
} // namespace volt::gfx

#endif
