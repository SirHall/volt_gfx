#pragma once
#ifndef VOLT_GFX_TEXTURE_HPP
#define VOLT_GFX_TEXTURE_HPP

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <vector>

#include "volt/gfx/extern/stb_image.h"

#include "volt/gfx/GLImport.hpp"

namespace volt::gfx
{
    class Texture
    {
    private:
        std::vector<std::uint8_t> imageData;
        std::uint_fast32_t        height;
        // We could calculate this from imageData.size() and height, but I'm
        // trying to avoid divisions
        std::uint_fast32_t width;
        std::uint32_t      spriteID;

        bool   loaded = false;
        GLuint texGlId;

    public:
        Texture();
        Texture(const std::vector<std::uint8_t> &data);
        Texture(std::vector<std::uint8_t> &&data);
        Texture(const Texture &other);
        Texture(Texture &&other);
        Texture &operator=(const Texture &other);
        Texture &operator=(Texture &&other);
        ~Texture();

        static Texture LoadFromFile(std::string path);

        inline bool IsLoadedIntoVRAM() { return this->loaded; }

        void LoadIntoVRAM();

        void Use(unsigned int unitIndex);
    };
} // namespace volt::gfx

#endif
