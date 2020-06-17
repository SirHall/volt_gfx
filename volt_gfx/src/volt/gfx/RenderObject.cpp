#include "volt/gfx/RenderObject.hpp"

#include <cassert>

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
    for (std::size_t i = 0; i < this->texUnits.size(); i++)
    {
        if (this->texUnits[i].has_value())
        {
            auto opt = this->texUnits[i];
            opt->Use((unsigned int)i);
        }
    }
}

Material & RenderObject::GetMaterial() { return this->material; }
Mesh &     RenderObject::GetMesh() { return this->mesh; }
Transform &RenderObject::GetTransform() { return this->transform; }

void RenderObject::SetTexture(Texture tex, std::uint8_t texUnit)
{
    assert(texUnit < 16);
    this->texUnits[texUnit] = std::optional<Texture>(tex);
}

void RenderObject::ClearTexture(std::uint8_t texUnit)
{
    assert(texUnit < 16);
    this->texUnits[texUnit] = std::optional<Texture>();
}

bool RenderObject::HasTexture(std::uint8_t texUnit)
{
    assert(texUnit < 16);
    return this->texUnits[texUnit].has_value();
}