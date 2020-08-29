#include "volt/gfx/RenderObject.hpp"

#include <cassert>

using namespace volt::gfx;

RenderObject::RenderObject(Material mat, Mesh m) : material(mat), mesh(m) {}

RenderObject::RenderObject(const RenderObject &other)
    : material(other.material), mesh(other.mesh)
{
}

RenderObject &RenderObject::operator=(const RenderObject &other)
{
    this->material = other.material;
    this->mesh     = other.mesh;
    return *this;
}

RenderObject::RenderObject(RenderObject &&other)
    : material(std::move(other.material)), mesh(std::move(other.mesh))
{
}

RenderObject &RenderObject::operator=(RenderObject &&other)
{
    this->material = std::move(other.material);
    this->mesh     = std::move(other.mesh);
    return *this;
}

RenderObject::~RenderObject() {}

void RenderObject::Bind() const
{
    this->mesh.Bind();
    this->material.Bind();
    for (std::size_t i = 0; i < this->texUnits.size(); i++)
    {
        if (this->texUnits[i].has_value())
        {
            auto opt = this->texUnits[i];
            opt->Use((unsigned int)i);
        }
    }
}

Material &RenderObject::GetMaterial() { return this->material; }
Mesh &    RenderObject::GetMesh() { return this->mesh; }

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

bool RenderObject::IsValid() const
{
    return this->IsShaderValid() && this->IsMeshValid();
}

bool RenderObject::IsShaderValid() const { return this->material.IsValid(); }

bool RenderObject::IsMeshValid() const { return this->mesh.IsValid(); }