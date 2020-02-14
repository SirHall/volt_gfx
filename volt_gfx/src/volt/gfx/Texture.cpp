#include "volt/gfx/Texture.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;

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

volt::gfx::Texture::~Texture() { gl::DeleteTextures(1, &this->texGlId); }

Texture Texture::LoadFromFile(std::string path)
{
    int      width = 0, height = 0, colorChannelsCount = 0;
    stbi_uc *imageData =
        stbi_load(path.c_str(), &width, &height, &colorChannelsCount, 0);
    std::size_t dataLen = (std::size_t)(width * height * colorChannelsCount);

    Texture tex   = Texture();
    tex.width     = width;
    tex.height    = height;
    tex.imageData = std::vector<std::uint8_t>();
    tex.imageData.resize(dataLen);
    for (std::size_t i = 0; i < dataLen; i++)
        tex.imageData[i] = imageData[i];
    stbi_image_free(imageData);
    return tex;
}

void Texture::LoadIntoVRAM()
{
    GLCall(gl::GenTextures(1, &this->texGlId));
    GLCall(gl::BindTexture(gl::TEXTURE_2D, this->texGlId));
    GLCall(gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, this->width,
                          this->height, 0, gl::RGB, gl::UNSIGNED_BYTE,
                          this->imageData.data()));
    GLCall(gl::GenerateMipmap(gl::TEXTURE_2D));

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S,
                             gl::CLAMP_TO_BORDER));
    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T,
                             gl::CLAMP_TO_BORDER));
    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER,
                             gl::NEAREST_MIPMAP_LINEAR));
    GLCall(
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST));

    this->loaded = true;
}

void Texture::Use(unsigned int unitIndex)
{
    assert(unitIndex < 16);
    GLCall(gl::ActiveTexture(gl::TEXTURE0 + unitIndex));
    GLCall(gl::BindTexture(gl::TEXTURE_2D, this->texGlId));
}