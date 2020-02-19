#include "volt/gfx/RenderObject.hpp"

using namespace volt::gfx;

RenderObject::RenderObject(Material mat, Mesh m, Transform t)
    : material(mat), mesh(m), transform(t)
{
}

RenderObject::RenderObject(const RenderObject &other)
    : material(other.material), mesh(other.mesh), transform(other.transform)
{
}

RenderObject &RenderObject::operator=(const RenderObject &other)
{
    this->material  = other.material;
    this->mesh      = other.mesh;
    this->transform = other.transform;
    return *this;
}

RenderObject::RenderObject(RenderObject &&other)
    : material(std::move(other.material)), mesh(std::move(other.mesh)),
      transform(std::move(other.transform))
{
}

RenderObject &RenderObject::operator=(RenderObject &&other)
{
    this->material  = std::move(other.material);
    this->mesh      = std::move(other.mesh);
    this->transform = std::move(other.transform);
    return *this;
}

RenderObject::~RenderObject() {}

void RenderObject::Bind() const
{
    this->material.Bind();
    this->mesh.Bind();
}

Material & RenderObject::GetMaterial() { return this->material; }
Mesh &     RenderObject::GetMesh() { return this->mesh; }
Transform &RenderObject::GetTransform() { return this->transform; }