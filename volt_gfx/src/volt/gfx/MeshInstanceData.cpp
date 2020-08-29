#include "volt/gfx/MeshInstanceData.hpp"
#include "volt/gfx/VAOGen.hpp"
#include "volt/gfx/VertAttribData.hpp"

using namespace volt::gfx;

MeshInstanceData::MeshInstanceData() : transform(1) {}
MeshInstanceData::MeshInstanceData(glm::mat4 transformMat)
    : transform(transformMat)
{
}

MeshInstanceData::MeshInstanceData(const MeshInstanceData &other)
    : transform(other.transform)
{
}

MeshInstanceData &MeshInstanceData::operator=(const MeshInstanceData &other)
{
    this->transform = other.transform;
    return *this;
}

MeshInstanceData::~MeshInstanceData() {}

template <>
GLuint volt::gfx::GenVBO<MeshInstanceData>(GLuint attribIndex,
                                           std::vector<VertAttribData> &verts)
{
    verts.push_back(
        VertAttribData("gfxTransform", attribIndex, sizeof(MeshInstanceData),
                       offsetof(MeshInstanceData, transform), true));
    verts.back().SetGenerator<glm::mat4>();
    attribIndex += VAOGen::AttribTypeSize<glm::mat4>();
    return attribIndex;
}