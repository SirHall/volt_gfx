#pragma once
#ifndef VOLT_GFX_SPRITE_HPP
#define VOLT_GFX_SPRITE_HPP

#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Texture.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace volt::gfx
{
    class Sprite
    {
    private:
        glm::vec4 srcR;
        glm::vec2 destR;

        Mesh mesh;

    public:
        Sprite(glm::vec4 srcRect, glm::vec2 destRect);
        Sprite(const Sprite &other);
        // Sprite(Sprite &&other);
        Sprite &operator=(const Sprite &other);
        // Sprite &operator=(Sprite &&other);
        ~Sprite();

        static Mesh CreateMesh(glm::vec4 srcRect, glm::vec2 destRect,
                               bool center = true);
        static Mesh CreateMesh(glm::vec4 srcRect, glm::vec4 destRect);

        inline glm::vec4   GetSrcRect() { return this->srcR; }
        inline glm::vec2   GetDestRect() { return this->destR; }
        inline Mesh const &GetMesh() { return this->mesh; }
    };
} // namespace volt::gfx

#endif
