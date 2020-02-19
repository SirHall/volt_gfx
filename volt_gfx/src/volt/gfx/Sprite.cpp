#include "volt/gfx/Sprite.hpp"
#include "volt/gfx/Vertex.hpp"

using namespace volt::gfx;

Sprite::Sprite(glm::vec4 srcRect, glm::vec2 destRect)
    : srcR(srcRect), destR(destRect), mesh()
{
    this->mesh = std::move(Sprite::CreateMesh(srcRect, destRect));
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

Mesh Sprite::CreateMesh(glm::vec4 srcRect, glm::vec2 destRect)
{
    float xOff = destRect.x * 0.5f, yOff = destRect.y * 0.5f;
    Mesh  mesh;
    mesh.CreateMesh(
        {
            Vertex(-xOff, yOff, 0.0f, srcRect.x, srcRect.y), // 0 - Top Left
            Vertex(-xOff, -yOff, 0.0f, srcRect.x,
                   srcRect.y + srcRect.w), // 1 - Bottom Left
            Vertex(xOff, -yOff, 0.0f, srcRect.w + srcRect.y,
                   srcRect.y + srcRect.w), // 2 - Bottom Right
            Vertex(xOff, yOff, 0.0f, srcRect.x + srcRect.z,
                   srcRect.y), // 3 - Top Right
        },
        {0, 1, 3, 1, 2, 3});
    return std::move(mesh);
}