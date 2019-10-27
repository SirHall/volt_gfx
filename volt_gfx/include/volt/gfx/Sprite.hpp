#pragma once
#ifndef VOLT_GFX_SPRITE_HPP
#define VOLT_GFX_SPRITE_HPP

#include "volt/gfx/Texture.hpp"

#include <glm/vec4.hpp>

#include <memory>

class Sprite
{
private:
    std::shared_ptr<Texture> tex;
    glm::vec4                srcRect;

protected:
public:
    Sprite();
    Sprite(const Sprite &other);
    // Sprite(Sprite &&other);
    Sprite &operator=(const Sprite &other);
    // Sprite &operator=(Sprite &&other);
    ~Sprite();
};

#endif
