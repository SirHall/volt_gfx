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

namespace volt::gfx
{
    class Texture
    {
    private:
        std::shared_ptr<GLuint> texID;
        GLsizei                 width, height;

        void CreateTexture(GLsizei width, GLsizei height, void const *data);

    public:
        Texture(Image const &image);
        Texture(GLsizei width, GLsizei height);

        Texture(const Texture &other);
        Texture(Texture &&other);
        Texture &operator=(const Texture &other);
        Texture &operator=(Texture &&other);
        ~Texture();

        void Use(unsigned int unitIndex);

        void Bind();
        void Unbind();

        inline GLuint GetTexID() { return *this->texID; }

        void GenerateMipmap();

        inline GLsizei GetWidth() { return this->width; }
        inline GLsizei GetHeight() { return this->height; }
    };
} // namespace volt::gfx

#endif
