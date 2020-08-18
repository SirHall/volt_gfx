#include "volt/gfx/MeshInstanceData.hpp"
#include "volt/gfx/VAOGen.hpp"
#include "volt/gfx/VertAttribData.hpp"

using namespace volt::gfx;

MeshInstanceData::MeshInstanceData() {}

MeshInstanceData::MeshInstanceData(const MeshInstanceData &other) {}

MeshInstanceData &MeshInstanceData::operator=(const MeshInstanceData &other)
{
    return *this;
}

MeshInstanceData::~MeshInstanceData() {}

template <>
GLuint volt::gfx::GenVBO<MeshInstanceData>(GLuint attribIndex,
                                           std::vector<VertAttribData> &verts)
{
    verts.push_back(VertAttribData("gfxTransform", attribIndex,
                                   sizeof(MeshInstanceData),
                                   offsetof(MeshInstanceData, transform)));
    verts.back().SetGenerator<glm::vec4>();
    attribIndex = VAOGen::AttribTypeSize<glm::mat4>();
    return attribIndex;
}