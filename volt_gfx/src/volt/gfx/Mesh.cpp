#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/GLUtilities.hpp"

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

Mesh::Mesh() : vao(0), vbo(0), ibo(0) {}

Mesh::Mesh(const Mesh &other) : vao(0), vbo(0), ibo(0)
{
    this->CreateMesh(other.vertices, other.indices);
}

Mesh &Mesh::operator=(const Mesh &other)
{
    vao = vbo = ibo = 0;
    this->CreateMesh(other.vertices, other.indices);

    return *this;
}

Mesh::Mesh(Mesh &&other)
{
    vao       = other.vao;
    vbo       = other.vbo;
    ibo       = other.ibo;
    other.vao = other.vbo = other.ibo = 0; // Clear other

    vertices = std::move(other.vertices);
    indices  = std::move(other.indices);
}

Mesh &Mesh::operator=(Mesh &&other)
{
    vao       = other.vao;
    vbo       = other.vbo;
    ibo       = other.ibo;
    other.vao = other.vbo = other.ibo = 0; // Clear other

    vertices = std::move(other.vertices);
    indices  = std::move(other.indices);

    return *this;
}

Mesh::~Mesh() { ClearMesh(); }

void Mesh::CreateMesh(std::vector<Vertex> const &       vertices,
                      std::vector<std::uint32_t> const &indices)
{
    this->vertices = vertices;
    this->indices  = indices;
    // Vertex buffer
    GLCall(gl::GenBuffers(1, &this->vbo));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, this->vbo));
    GLCall(gl::BufferData(gl::ARRAY_BUFFER, VecTotalSize(this->vertices),
                          this->vertices.data(), gl::STATIC_DRAW));

    // Setup vertex array object
    this->vao = Vertex::GenerateVAO();

    // Index buffer
    GLCall(gl::GenBuffers(1, &this->ibo));
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, this->ibo));
    GLCall(gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, VecTotalSize(this->indices),
                          this->indices.data(), gl::STATIC_DRAW));

    // Unbind all buffers
    GLCall(gl::BindVertexArray(0));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, 0));
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0));
}

void Mesh::RenderMesh()
{
    if (this->vao == 0)
        std::cout << "vao == 0" << std::endl;
    if (this->vbo == 0)
        std::cout << "vbo == 0" << std::endl;
    if (this->ibo == 0)
        std::cout << "ibo == 0" << std::endl;

    GLCall(gl::BindVertexArray(vao));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, vbo)); // Probably not needed
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ibo));

    // The draw call
    GLCall(
        gl::DrawElements(gl::TRIANGLES, indices.size(), gl::UNSIGNED_INT, 0));

    GLCall(gl::BindVertexArray(0));
    GLCall(gl::BindBuffer(gl::ARRAY_BUFFER, 0)); // Probably not needed
    GLCall(gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0));
}

void Mesh::ClearMesh()
{
    GLCall(gl::DeleteBuffers(1, &ibo));
    ibo = 0;
    GLCall(gl::DeleteBuffers(1, &vbo));
    vbo = 0;
    GLCall(gl::DeleteVertexArrays(1, &vao));
    vao = 0;
    indices.clear();
    vertices.clear();
}

GLuint Mesh::GetVAO() const { return this->vao; }

GLuint Mesh::GetVBO() const { return this->vbo; }

GLuint Mesh::GetIBO() const { return this->ibo; }

const std::vector<Vertex> &Mesh::GetVertices() const { return this->vertices; }

const std::vector<std::uint32_t> &Mesh::GetIndices() const
{
    return this->indices;
}
