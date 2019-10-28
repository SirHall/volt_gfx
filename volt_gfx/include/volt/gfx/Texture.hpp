#pragma once
#ifndef VOLT_GFX_TEXTURE_HPP
#define VOLT_GFX_TEXTURE_HPP

#include <cstdint>
#include <vector>

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

    protected:
    public:
        Texture();
        Texture(const std::vector<std::uint8_t> &data);
        Texture(std::vector<std::uint8_t> &&data);
        Texture(const Texture &other);
        Texture(Texture &&other);
        Texture &operator=(const Texture &other);
        Texture &operator=(Texture &&other);
        ~Texture();
    };
} // namespace volt::gfx

#endif
