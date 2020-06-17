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
    GLCall(glActiveTexture(GL_TEXTURE0 + unitIndex));
    this->Bind();
}

void Texture::Bind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, this->texData->texID));
}

void Texture::Unbind() { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }

void Texture::CreateTexture(GLsizei width, GLsizei height, bool enableMipmaps,
                            void const *data)
{
    GLuint id = 0;

    GLCall(glGenTextures(1, &id));
    this->texData = std::move(std::shared_ptr<TexData>(
        new TexData(id, enableMipmaps), [=](TexData *ptr) {
            GLCall(glDeleteTextures(1, &ptr->texID));
            delete ptr;
        }));

    this->Bind();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, data));
    this->GenerateMipmap();

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                           this->texData->mipmaps ? GL_NEAREST_MIPMAP_LINEAR
                                                  : GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}

GLuint Texture::GetTexID() { return this->texData->texID; }

void Texture::GenerateMipmap()
{
    this->Bind();
    if (this->texData->mipmaps)
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

GLsizei Texture::GetWidth()
{
    GLsizei width = 0;
    GLCall(
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
    return width;
}

GLsizei Texture::GetHeight()
{
    GLsizei height = 0;
    GLCall(
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height));
    return height;
}

void Texture::Resize(GLsizei newWidth, GLsizei newHeight)
{
    this->Bind();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, newWidth, newHeight, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
}