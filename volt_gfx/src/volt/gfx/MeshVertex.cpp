#include "volt/gfx/MeshVertex.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/VAOGen.hpp"

#include <iostream>

using namespace volt::gfx;

MeshVertex::MeshVertex() {}

MeshVertex::MeshVertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                       GLfloat uv_x, GLfloat uv_y)
{
    position[0] = pos_x;
    position[1] = pos_y;
    position[2] = pos_z;

    uv[0] = uv_x;
    uv[1] = uv_y;
}

MeshVertex::MeshVertex(const MeshVertex &other)
{
    this->position[0] = other.position[0];
    this->position[1] = other.position[1];
    this->position[2] = other.position[2];

    this->uv[0] = other.uv[0];
    this->uv[1] = other.uv[1];
}

MeshVertex &MeshVertex::operator=(const MeshVertex &other)
{
    this->position[0] = other.position[0];
    this->position[1] = other.position[1];
    this->position[2] = other.position[2];

    this->uv[0] = other.uv[0];
    this->uv[1] = other.uv[1];

    return *this;
}

MeshVertex::~MeshVertex() {}

template <>
GLuint volt::gfx::GenVBO<MeshVertex>(GLuint attribIndex)
{
    attribIndex = VAOGen::GenVertAttrib<glm::vec3>(
        "gfxPos", attribIndex, sizeof(MeshVertex),
        offsetof(MeshVertex, position));
    attribIndex = VAOGen::GenVertAttrib<glm::vec2>(
        "gfxUV", attribIndex, sizeof(MeshVertex), offsetof(MeshVertex, uv));
    return attribIndex;
}