#include "volt/gfx/Sprite.hpp"

volt::gfx::Sprite::Sprite() {}

volt::gfx::Sprite::Sprite(const Sprite &other)
{
    this->srcRect = other.srcRect;
    this->tex     = other.tex;
}

// Sprite::Sprite(Sprite &&other)
// {
//     this->srcRect = other.srcRect;
//     this->tex     = std::move(other.tex);
// }

volt::gfx::Sprite &volt::gfx::Sprite::operator=(const Sprite &other)
{
    return *this;
}

// Sprite &Sprite::operator=(Sprite &&other) { return *this; }

volt::gfx::Sprite::~Sprite() {}
