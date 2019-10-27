#include "volt/gfx/Texture.hpp"

Texture::Texture() : height(0), width(0) {}

Texture::Texture(const Texture &other)
{
    this->height                           = other.height;
    this->width                            = other.width;
    std::vector<std::uint8_t>::iterator it = this->imageData.begin();
    auto insert_it = std::insert_iterator(this->imageData, it);
    std::copy(other.imageData.begin(), other.imageData.end(), insert_it);
}

Texture::Texture(const std::vector<std::uint8_t> &data)
{
    std::copy(data.begin(), data.end(), this->imageData.begin());
}

Texture::Texture(std::vector<std::uint8_t> &&data)
{
    this->imageData = std::move(data);
}

Texture::Texture(Texture &&other)
{
    this->height    = other.height;
    this->width     = other.width;
    this->imageData = std::move(other.imageData);
}

Texture &Texture::operator=(const Texture &other)
{
    this->height = other.height;
    this->width  = other.width;
    std::copy(other.imageData.begin(), other.imageData.end(),
              this->imageData.begin());
    return *this;
}

Texture &Texture::operator=(Texture &&other)
{
    this->height    = other.height;
    this->width     = other.width;
    this->imageData = std::move(other.imageData);
    return *this;
}

Texture::~Texture() {}
