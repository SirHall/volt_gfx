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

        void LoadIntoVRAM();

    public:
        Texture(Image const &image);
        Texture(const Texture &other);
        Texture(Texture &&other);
        Texture &operator=(const Texture &other);
        Texture &operator=(Texture &&other);
        ~Texture();

        void Use(unsigned int unitIndex);

        inline GLuint GetTexID() { return *this->texID; }
    };
} // namespace volt::gfx

#endif
