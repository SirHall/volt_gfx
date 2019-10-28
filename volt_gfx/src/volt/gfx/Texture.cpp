#include "volt/gfx/Texture.hpp"

volt::gfx::Texture::Texture() : height(0), width(0) {}

volt::gfx::Texture::Texture(const volt::gfx::Texture &other)
{
    this->height                           = other.height;
    this->width                            = other.width;
    std::vector<std::uint8_t>::iterator it = this->imageData.begin();
    auto insert_it = std::insert_iterator(this->imageData, it);
    std::copy(other.imageData.begin(), other.imageData.end(), insert_it);
}

volt::gfx::Texture::Texture(const std::vector<std::uint8_t> &data)
{
    std::copy(data.begin(), data.end(), this->imageData.begin());
}

volt::gfx::Texture::Texture(std::vector<std::uint8_t> &&data)
{
    this->imageData = std::move(data);
}

volt::gfx::Texture::Texture(volt::gfx::Texture &&other)
{
    this->height    = other.height;
    this->width     = other.width;
    this->imageData = std::move(other.imageData);
}

volt::gfx::Texture &
    volt::gfx::Texture::operator=(const volt::gfx::Texture &other)
{
    this->height = other.height;
    this->width  = other.width;
    std::copy(other.imageData.begin(), other.imageData.end(),
              this->imageData.begin());
    return *this;
}

volt::gfx::Texture &volt::gfx::Texture::operator=(volt::gfx::Texture &&other)
{
    this->height    = other.height;
    this->width     = other.width;
    this->imageData = std::move(other.imageData);
    return *this;
}

volt::gfx::Texture::~Texture() {}
