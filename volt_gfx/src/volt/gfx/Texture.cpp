#include "volt/gfx/Texture.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;

Texture::Texture(Image const &image)
{
    this->CreateTexture(image.GetWidth(), image.GetHeight(),
                        static_cast<void const *>(image.GetImageData().data()));
}

Texture::Texture(GLsizei width, GLsizei height)
{
    this->CreateTexture(width, height, nullptr);
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
    this->Bind();
    GLCall(gl::ActiveTexture(gl::TEXTURE0 + unitIndex));
}

void Texture::Bind() { GLCall(gl::BindTexture(gl::TEXTURE_2D, *this->texID)); }

void Texture::Unbind() { GLCall(gl::BindTexture(gl::TEXTURE_2D, 0)); }

void Texture::CreateTexture(GLsizei width, GLsizei height, void const *data)
{
    this->width  = width;
    this->height = height;
    GLuint id    = 0;

    GLCall(gl::GenTextures(1, &id));
    this->texID =
        std::move(std::shared_ptr<GLuint>(new GLuint(id), [=](GLuint *ptr) {
            GLCall(gl::DeleteTextures(1, ptr));
            delete ptr;
        }));

    this->Bind();
    GLCall(gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0,
                          gl::RGBA, gl::UNSIGNED_BYTE, data));
    this->GenerateMipmap();

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S,
                             gl::CLAMP_TO_EDGE));
    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T,
                             gl::CLAMP_TO_EDGE));

    GLCall(gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER,
                             gl::NEAREST_MIPMAP_LINEAR));
    GLCall(
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST));
}

void Texture::GenerateMipmap()
{
    this->Bind();
    GLCall(gl::GenerateMipmap(gl::TEXTURE_2D));
}