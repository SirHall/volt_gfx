#include "volt/gfx/MeshInstanceData.hpp"

#include "volt/gfx/VAOGen.hpp"

using namespace volt::gfx;

MeshInstanceData::MeshInstanceData() {}

MeshInstanceData::MeshInstanceData(const MeshInstanceData &other) {}

MeshInstanceData &MeshInstanceData::operator=(const MeshInstanceData &other)
{
    return *this;
}

MeshInstanceData::~MeshInstanceData() {}

template <>
GLuint volt::gfx::GenVBO<MeshInstanceData>(GLuint vbo, GLuint attribIndex)
{
    attribIndex = VAOGen::GenVertAttrib<glm::mat4>(
        "gfxTransform", attribIndex, sizeof(MeshInstanceData),
        offsetof(MeshInstanceData, transform));
    return attribIndex;
}