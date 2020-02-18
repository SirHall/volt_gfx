#pragma once
#ifndef VOLT_GFX_FRAMEBUFFER_HPP
#define VOLT_GFX_FRAMEBUFFER_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Texture.hpp"

#include <memory>

namespace volt::gfx
{
    struct FramebufferTarget
    {
    private:
        GLenum const val;

    public:
        FramebufferTarget(GLenum v);
        inline GLenum Get() { return this->val; }

        static FramebufferTarget Read();
        static FramebufferTarget Write();
        static FramebufferTarget ReadWrite();
    };

    class Framebuffer
    {
    private:
        std::shared_ptr<GLuint> fbo;

    public:
        Framebuffer();
        Framebuffer(const Framebuffer &other);
        Framebuffer &operator=(const Framebuffer &other);
        Framebuffer(Framebuffer &&other);
        Framebuffer &operator=(Framebuffer &&other);
        ~Framebuffer();

        static void BindDefaultFramebuffer();

        bool IsValid();
        bool IsFrameBufferComplete();

        void BindReadTarget();
        void BindWriteTarget();
        void BindReadWriteTarget();

        void AttachTexture(Texture tex, FramebufferTarget target,
                           std::uint8_t attachmentIndex = 0);
    };

} // namespace volt::gfx
#endif
