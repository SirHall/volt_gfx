#include "volt/gfx/Texture.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;
using namespace volt::gfx::internal;

TexData::TexData(GLuint textureID, bool enableMipmaps)
    : texID(textureID), mipmaps(enableMipmaps)
{
}

Texture::Texture(Image const &image, bool enableMipmaps)
{
    this->CreateTexture(image.GetWidth(), image.GetHeight(), enableMipmaps,
                        static_cast<void const *>(image.GetImageData().data()));
}

Texture::Texture(GLsizei width, GLsizei height, bool enableMipmaps)
{
    this->CreateTexture(width, height, enableMipmaps, nullptr);
}

Texture::Texture(const Texture &other) : texData(other.texData) {}

Texture &Texture::operator=(const Texture &other)
{
    this->texData = other.texData;
    return *this;
}

Texture::Texture(Texture &&other) : texData(std::move(other.texData)) {}

Texture &Texture::operator=(Texture &&other)
{
    this->texData = std::move(other.texData);
    return *this;
}

volt::gfx::Texture::~Texture() {}

void Texture::Use(unsigned int unitIndex)
{
    assert(unitIndex < 16);
    GLCall(gl::ActiveTexture(gl::TEXTURE0 + unitIndex));
    this->Bind();
}

void Texture::Bind()
{
    GLCall(gl::BindTexture(gl::TEXTURE_2D, this->texData->texID));
}

void Texture::Unbind() { GLCall(gl::BindTexture(gl::TEXTURE_2D, 0)); }

void Texture::CreateTexture(GLsizei width, GLsizei height, bool enableMipmaps,
                            void const *data)
{
    GLuint id = 0;

    GLCall(gl::GenTextures(1, &id));
    this->texData = std::move(std::shared_ptr<TexData>(
        new TexData(id, enableMipmaps), [=](TexData *ptr) {
            GLCall(gl::DeleteTextures(1, &ptr->texID));
            delete ptr;
        }));

    this->Bind();
    GLCall(gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA16F, width, height, 0,
                          gl::RGBA, gl::UNSIGNED_BYTE, data));
    this->GenerateMipmap();

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S,
                             gl::CLAMP_TO_EDGE));
    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T,
                             gl::CLAMP_TO_EDGE));

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER,
                             this->texData->mipmaps ? gl::NEAREST_MIPMAP_LINEAR
                                                    : gl::NEAREST));
    GLCall(
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST));
}

GLuint Texture::GetTexID() { return this->texData->texID; }

void Texture::GenerateMipmap()
{
    this->Bind();
    if (this->texData->mipmaps)
        GLCall(gl::GenerateMipmap(gl::TEXTURE_2D));
}

GLsizei Texture::GetWidth()
{
    GLsizei width = 0;
    gl::GetTexLevelParameteriv(gl::TEXTURE_2D, 0, gl::TEXTURE_WIDTH, &width);
    return width;
}

GLsizei Texture::GetHeight()
{
    GLsizei height = 0;
    gl::GetTexLevelParameteriv(gl::TEXTURE_2D, 0, gl::TEXTURE_HEIGHT, &height);
    return height;
}
