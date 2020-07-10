#include "volt/gfx/Sprite.hpp"
#include "volt/gfx/MeshVertex.hpp"

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

Mesh Sprite::CreateMesh(glm::vec4 srcRect, glm::vec2 destRect, bool center)
{
    // This could probably be cleaned up
    float xMin = center ? -destRect.x * 0.5f : 0.0f,
          xMax = center ? destRect.x : destRect.x,
          yMin = center ? -destRect.y * 0.5f : 0.0f,
          yMax = center ? destRect.y : destRect.y;

    return std::move(CreateMesh(srcRect, glm::vec4(xMin, yMin, xMax, yMax)));
}

Mesh Sprite::CreateMesh(glm::vec4 srcRect, glm::vec4 destRect)
{
    Mesh mesh;
    mesh.CreateMesh(
        {
            MeshVertex(destRect.x, destRect.y + destRect.w, 0.0f, srcRect.x,
                       srcRect.y), // 0 - Left Top
            MeshVertex(destRect.x, destRect.y, 0.0f, srcRect.x,
                       srcRect.y + srcRect.w), // 1 - Left Bottom
            MeshVertex(destRect.x + destRect.z, destRect.y, 0.0f,
                       srcRect.x + srcRect.z,
                       srcRect.y + srcRect.w), // 2 - Right Bottom
            MeshVertex(destRect.x + destRect.z, destRect.y + destRect.w, 0.0f,
                       srcRect.x + srcRect.z, srcRect.y), // 3 - Right Top
        },
        {0, 1, 3, 1, 2, 3});
    return std::move(mesh);
}