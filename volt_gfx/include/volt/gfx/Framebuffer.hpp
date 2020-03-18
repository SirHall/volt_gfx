#pragma once
#ifndef VOLT_GFX_FRAMEBUFFER_HPP
#define VOLT_GFX_FRAMEBUFFER_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Texture.hpp"

#include <array>
#include <memory>
#include <optional>

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
        std::shared_ptr<GLuint>                fbo;
        std::array<std::optional<Texture>, 32> textures;

    public:
        Framebuffer();
        Framebuffer(const Framebuffer &other);
        Framebuffer &operator=(const Framebuffer &other);
        Framebuffer(Framebuffer &&other);
        Framebuffer &operator=(Framebuffer &&other);
        ~Framebuffer();

        inline bool operator==(const Framebuffer &rhs)
        {
            return fbo.get() == rhs.fbo.get();
        }
        inline bool operator!=(const Framebuffer &rhs)
        {
            return !(*this == rhs);
        }

        static void BindDefaultFramebuffer();

        bool IsValid();
        bool IsFrameBufferComplete();

        void BindReadTarget(std::uint8_t attachmentIndex = 0);
        void BindWriteTarget(std::uint8_t attachmentIndex = 0);
        void BindReadWriteTarget(std::uint8_t attachmentIndex = 0);

        void BindTex(std::uint8_t attachmentIndex = 0);

        void AttachTexture(Texture tex, FramebufferTarget target,
                           std::uint8_t attachmentIndex = 0);

        std::optional<Texture> GetTexture(std::uint8_t attachmentIndex = 0);
    };

} // namespace volt::gfx
#endif
