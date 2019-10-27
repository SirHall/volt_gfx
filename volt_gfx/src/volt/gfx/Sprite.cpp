#include "volt/gfx/Sprite.hpp"

Sprite::Sprite() {}

Sprite::Sprite(const Sprite &other)
{
    this->srcRect = other.srcRect;
    this->tex     = other.tex;
}

// Sprite::Sprite(Sprite &&other)
// {
//     this->srcRect = other.srcRect;
//     this->tex     = std::move(other.tex);
// }

Sprite &Sprite::operator=(const Sprite &other) { return *this; }

// Sprite &Sprite::operator=(Sprite &&other) { return *this; }

Sprite::~Sprite() {}
