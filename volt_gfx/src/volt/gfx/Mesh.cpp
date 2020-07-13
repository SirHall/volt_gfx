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
std::size_t VecTotalSize(const std::vector<T> &vec)
{
    return GetVecTypeSize(vec) * vec.size();
}

Mesh::Mesh() : vao() {}

Mesh::Mesh(const Mesh &other) : vao()
{
    other.GetVAO().GetIBO().CopyTo(this->vao.GetIBO());
    other.GetVAO().GetVertVBO().CopyTo(this->vao.GetVertVBO());
    other.GetVAO().GetInstVBO().CopyTo(this->vao.GetInstVBO());
}

Mesh &Mesh::operator=(const Mesh &other)
{
    other.GetVAO().GetIBO().CopyTo(this->vao.GetIBO());
    other.GetVAO().GetVertVBO().CopyTo(this->vao.GetVertVBO());
    other.GetVAO().GetInstVBO().CopyTo(this->vao.GetInstVBO());
    return *this;
}

Mesh::Mesh(Mesh &&other)
    : vao(std::move(other.vao)), vertices(std::move(other.vertices)),
      indices(std::move(other.indices))
{
}

Mesh &Mesh::operator=(Mesh &&other)
{
    vao      = std::move(other.vao);
    vertices = std::move(other.vertices);
    indices  = std::move(other.indices);

    return *this;
}

Mesh::~Mesh() {}

void Mesh::CreateMesh(std::vector<MeshVertex> const &   vertices,
                      std::vector<std::uint32_t> const &indices)
{
    this->vertices = vertices;
    this->indices  = indices;
    this->vao.GetVertVBO().SetData(this->vertices);
    this->vao.GetIBO().SetData(this->indices);
}

void Mesh::RenderMesh()
{
    if (!this->vao.IsValid())
        std::cerr << "VAO is invalid" << std::endl;

    this->vao.Bind();

    // The draw call
    GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                          GL_UNSIGNED_INT, 0));
}

const std::vector<MeshVertex> &Mesh::GetVertices() const
{
    return this->vertices;
}

const std::vector<std::uint32_t> &Mesh::GetIndices() const
{
    return this->indices;
}

bool Mesh::IsValid() const { return this->vao.IsValid(); }

void Mesh::Bind() const { this->vao.Bind(); }

VAO<MeshVertex, MeshInstanceData> &Mesh::GetVAO() { return this->vao; }

VAO<MeshVertex, MeshInstanceData> const &Mesh::GetVAO() const
{
    return this->vao;
}
