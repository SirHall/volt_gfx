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
void volt::gfx::GenVBO<MeshInstanceData>(GLuint vbo)
{
    GLuint vertIndex = 0;

    vertIndex = VAOGen::GenVertAttrib<glm::mat4>(
        "gfxTransform", vertIndex, sizeof(MeshInstanceData),
        offsetof(MeshInstanceData, transform));
}