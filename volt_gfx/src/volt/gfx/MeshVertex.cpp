#include "volt/gfx/MeshVertex.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/VAOGen.hpp"

#include <iostream>

using namespace volt::gfx;

MeshVertex::MeshVertex() {}

MeshVertex::MeshVertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                       GLfloat uv_x, GLfloat uv_y)
{
    position.x = pos_x;
    position.y = pos_y;
    position.z = pos_z;

    uv.x = uv_x;
    uv.y = uv_y;
}

MeshVertex::MeshVertex(const MeshVertex &other)
{
    this->position.x = other.position.x;
    this->position.y = other.position.y;
    this->position.z = other.position.z;

    this->uv.x = other.uv.x;
    this->uv.y = other.uv.y;
}

MeshVertex &MeshVertex::operator=(const MeshVertex &other)
{
    this->position.x = other.position.x;
    this->position.y = other.position.y;
    this->position.z = other.position.z;

    this->uv.x = other.uv.x;
    this->uv.y = other.uv.y;

    return *this;
}

MeshVertex::~MeshVertex() {}

template <>
GLuint volt::gfx::GenVBO<MeshVertex>(GLuint                       attribIndex,
                                     std::vector<VertAttribData> &verts)
{
    verts.push_back(VertAttribData("gfxPos", attribIndex, sizeof(MeshVertex),
                                   offsetof(MeshVertex, position)));
    verts.back().SetGenerator<glm::vec3>();
    attribIndex += VAOGen::AttribTypeSize<glm::vec3>();
    verts.push_back(VertAttribData("gfxUV", attribIndex, sizeof(MeshVertex),
                                   offsetof(MeshVertex, uv)));
    verts.back().SetGenerator<glm::vec2>();
    attribIndex += VAOGen::AttribTypeSize<glm::vec2>();

    return attribIndex;
}