#include "volt/gfx/Framebuffer.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;

FramebufferTarget::FramebufferTarget(GLenum v) : val(v) {}
FramebufferTarget FramebufferTarget::Read()
{
    return FramebufferTarget(GL_READ_FRAMEBUFFER);
}
FramebufferTarget FramebufferTarget::Write()
{
    return FramebufferTarget(GL_DRAW_FRAMEBUFFER);
}
FramebufferTarget FramebufferTarget::ReadWrite()
{
    return FramebufferTarget(GL_FRAMEBUFFER);
}

Framebuffer::Framebuffer()
    : fbo(std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint *ptr) {
          GLCall(glDeleteFramebuffers(1, ptr));
          delete ptr;
      }))
{
    GLCall(glGenFramebuffers(1, fbo.get()));
}

Framebuffer::Framebuffer(const Framebuffer &other) : fbo(other.fbo) {}

Framebuffer &Framebuffer::operator=(const Framebuffer &other)
{
    this->fbo = other.fbo;
    return *this;
}

Framebuffer::Framebuffer(Framebuffer &&other) : fbo(std::move(other.fbo)) {}

Framebuffer &Framebuffer::operator=(Framebuffer &&other)
{
    this->fbo = std::move(other.fbo);
    return *this;
}

Framebuffer::~Framebuffer() {}

void Framebuffer::BindDefaultFramebuffer()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool Framebuffer::IsValid() { return *this->fbo > 0; }

bool Framebuffer::IsFrameBufferComplete()
{
    this->BindReadWriteTarget();
    GLCall(bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                           GL_FRAMEBUFFER_COMPLETE);
    return complete;
}

void Framebuffer::BindReadTarget(std::uint8_t attachmentIndex)
{
    this->BindTex(attachmentIndex);
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, *this->fbo));
}

void Framebuffer::BindWriteTarget(std::uint8_t attachmentIndex)
{
    this->BindTex(attachmentIndex);
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *this->fbo));
}

void Framebuffer::BindReadWriteTarget(std::uint8_t attachmentIndex)
{
    this->BindTex(attachmentIndex);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, *this->fbo));
}

void Framebuffer::BindTex(std::uint8_t attachmentIndex)
{
    assert(attachmentIndex < 32);
    if (!this->textures[attachmentIndex].has_value())
        return;
    auto &tex = this->textures[attachmentIndex].value();
    tex.Bind();
}

void Framebuffer::AttachTexture(Texture tex, FramebufferTarget target,
                                std::uint8_t attachmentIndex)
{
    assert(attachmentIndex < 32);
    tex.Bind();
    this->BindReadWriteTarget();
    GLCall(glFramebufferTexture2D(target.Get(),
                                    GL_COLOR_ATTACHMENT0 + attachmentIndex,
                                    GL_TEXTURE_2D, tex.GetTexID(), 0));
    this->textures[attachmentIndex] = tex;
}

std::optional<Texture> Framebuffer::GetTexture(std::uint8_t attachmentIndex)
{
    return this->textures[attachmentIndex];
}