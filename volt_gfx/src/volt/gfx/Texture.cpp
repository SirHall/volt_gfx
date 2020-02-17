#include "volt/gfx/Texture.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;

Texture::Texture(Image const &image)
{
    GLuint id = 0;

    GLCall(gl::GenTextures(1, &id));
    GLCall(gl::BindTexture(gl::TEXTURE_2D, id));
    GLCall(gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, image.GetWidth(),
                          image.GetHeight(), 0, gl::RGBA, gl::UNSIGNED_BYTE,
                          image.GetImageData().data()));
    GLCall(gl::GenerateMipmap(gl::TEXTURE_2D));

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S,
                             gl::CLAMP_TO_EDGE));
    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T,
                             gl::CLAMP_TO_EDGE));

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER,
                             gl::NEAREST_MIPMAP_LINEAR));
    GLCall(
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST));

    this->texID =
        std::move(std::shared_ptr<GLuint>(new GLuint(id), [=](GLuint *ptr) {
            GLCall(gl::DeleteTextures(1, ptr));
            delete ptr;
        }));
}

Texture::Texture(const Texture &other) : texID(other.texID) {}

Texture &Texture::operator=(const Texture &other)
{
    this->texID = other.texID;
    return *this;
}

Texture::Texture(Texture &&other) : texID(std::move(other.texID)) {}

Texture &Texture::operator=(Texture &&other)
{
    this->texID = std::move(other.texID);
    return *this;
}

volt::gfx::Texture::~Texture() {}

void Texture::Use(unsigned int unitIndex)
{
    assert(unitIndex < 16);
    GLCall(gl::ActiveTexture(gl::TEXTURE0 + unitIndex));
    GLCall(gl::BindTexture(gl::TEXTURE_2D, *this->texID));
}