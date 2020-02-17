#pragma once
#ifndef VOLT_GFX_IMAGE_HPP
#define VOLT_GFX_IMAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "volt/gfx/GLImport.hpp"

namespace volt::gfx
{

    class Image
    {
    private:
        std::vector<std::uint8_t> imageData;
        GLsizei                   width;
        GLsizei                   height;

    public:
        Image(std::string const &path);
        Image(const std::vector<std::uint8_t> &data);
        Image(std::vector<std::uint8_t> &&data);
        Image(const Image &other);
        Image &operator=(const Image &other);
        Image(Image &&other);
        Image &operator=(Image &&other);
        ~Image();

        inline GLsizei GetWidth() const { return this->width; }
        inline GLsizei GetHeight() const { return this->height; }

        inline std::vector<std::uint8_t> const &GetImageData() const
        {
            return this->imageData;
        }
    };

} // namespace volt::gfx
#endif
