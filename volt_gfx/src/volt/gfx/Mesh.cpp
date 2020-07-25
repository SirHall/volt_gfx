#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/MeshInstanceData.hpp"
#include "volt/gfx/VAOGen.hpp"

#include <iostream>

using namespace volt::gfx;

template <typename T>
std::size_t GetVecTypeSize(const std::vector<T> &vec)
{
    return sizeof(T);
}

template <typename T>
std::size_t VecTotalSize(std::vector<T> const &vec)
{
    return GetVecTypeSize(vec) * vec.size();
}

Mesh::Mesh() : vao() {}

Mesh::Mesh(Mesh const &other) : vao()
{
    other.GetVAO().GetIBO().CopyTo(this->vao.GetIBO());
    other.GetVAO().GetVertVBO().CopyTo(this->vao.GetVertVBO());
    other.GetVAO().GetInstVBO().CopyTo(this->vao.GetInstVBO());
}

Mesh &Mesh::operator=(Mesh const &other)
{
    other.GetVAO().GetIBO().CopyTo(this->vao.GetIBO());
    other.GetVAO().GetVertVBO().CopyTo(this->vao.GetVertVBO());
    other.GetVAO().GetInstVBO().CopyTo(this->vao.GetInstVBO());
    return *this;
}

Mesh::Mesh(Mesh &&other) : vao(std::move(other.vao)) {}

Mesh &Mesh::operator=(Mesh &&other)
{
    this->vao = std::move(other.vao);
    return *this;
}

Mesh::~Mesh() {}

void Mesh::CreateMesh(std::vector<MeshVertex> const &   vertices,
                      std::vector<std::uint32_t> const &indices)
{
    this->vao.GetVertVBO().SetData(vertices);
    this->vao.GetIBO().SetData(indices);
}

std::vector<MeshVertex> Mesh::GetVertices() const
{
    return std::move(this->vao.GetVertVBO().GetData());
}

std::vector<std::uint32_t> Mesh::GetIndices() const
{
    return std::move(this->vao.GetIBO().GetData());
}

bool Mesh::IsValid() const { return this->vao.IsValid(); }

void Mesh::Bind() const { this->vao.Bind(); }

VAO<MeshVertex, MeshInstanceData> &Mesh::GetVAO() { return this->vao; }

VAO<MeshVertex, MeshInstanceData> const &Mesh::GetVAO() const
{
    return this->vao;
}
