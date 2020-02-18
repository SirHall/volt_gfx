#include "volt/gfx/Framebuffer.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cassert>

using namespace volt::gfx;

FramebufferTarget::FramebufferTarget(GLenum v) : val(v) {}
FramebufferTarget FramebufferTarget::Read()
{
    return FramebufferTarget(gl::READ_FRAMEBUFFER);
}
FramebufferTarget FramebufferTarget::Write()
{
    return FramebufferTarget(gl::DRAW_FRAMEBUFFER);
}
FramebufferTarget FramebufferTarget::ReadWrite()
{
    return FramebufferTarget(gl::FRAMEBUFFER);
}

Framebuffer::Framebuffer()
    : fbo(std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint *ptr) {
          GLCall(gl::DeleteFramebuffers(1, ptr));
          delete ptr;
      }))
{
    GLCall(gl::GenFramebuffers(1, fbo.get()));
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
    GLCall(gl::BindFramebuffer(gl::FRAMEBUFFER, 0));
}

bool Framebuffer::IsValid() { return *this->fbo > 0; }

bool Framebuffer::IsFrameBufferComplete()
{
    this->BindReadWriteTarget();
    GLCall(bool complete = gl::CheckFramebufferStatus(gl::FRAMEBUFFER) ==
                           gl::FRAMEBUFFER_COMPLETE);
    return complete;
}

void Framebuffer::BindReadTarget()
{
    GLCall(gl::BindFramebuffer(gl::READ_FRAMEBUFFER, *this->fbo));
}

void Framebuffer::BindWriteTarget()
{
    GLCall(gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, *this->fbo));
}

void Framebuffer::BindReadWriteTarget()
{
    GLCall(gl::BindFramebuffer(gl::FRAMEBUFFER, *this->fbo));
}

void Framebuffer::AttachTexture(Texture tex, FramebufferTarget target,
                                std::uint8_t attachmentIndex)
{
    assert(attachmentIndex < 32);
    tex.Bind();
    this->BindReadWriteTarget();
    GLCall(gl::FramebufferTexture2D(target.Get(),
                                    gl::COLOR_ATTACHMENT0 + attachmentIndex,
                                    gl::TEXTURE_2D, tex.GetTexID(), 0));
}