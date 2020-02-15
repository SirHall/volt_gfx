#include "volt/gfx/Sprite.hpp"
#include "volt/gfx/Vertex.hpp"

using namespace volt::gfx;

Sprite::Sprite(glm::vec4 srcRect, glm::vec2 destRect)
    : srcR(srcRect), destR(destRect), mesh()
{
    float xOff = destRect.x * 0.5f, yOff = destRect.y * 0.5f;

    this->mesh.CreateMesh(
        {
            Vertex(-xOff, yOff, 0.0f, 0.0f, 0.0f),  // 0 - Top Left
            Vertex(-xOff, -yOff, 0.0f, 0.0f, 1.0f), // 1 - Bottom Left
            Vertex(xOff, -yOff, 0.0f, 1.0f, 1.0f),  // 2 - Bottom Right
            Vertex(xOff, yOff, 0.0f, 1.0f, 0.0f),   // 3 - Top Right
        },
        {0, 1, 3, 1, 2, 3});
}

Sprite::Sprite(const Sprite &other)
{
    this->srcR  = other.srcR;
    this->destR = other.destR;
}

// Sprite::Sprite(Sprite &&other)
// {
//     this->srcRect = other.srcRect;
//     this->tex     = std::move(other.tex);
// }

Sprite &Sprite::operator=(const Sprite &other)
{
    this->srcR  = other.srcR;
    this->destR = other.destR;
    return *this;
}

// Sprite &Sprite::operator=(Sprite &&other) { return *this; }

Sprite::~Sprite() {}
