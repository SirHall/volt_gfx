#pragma once
#ifndef VOLT_GFX_RENDEROBJECT_HPP
#define VOLT_GFX_RENDEROBJECT_HPP

#include "volt/gfx/Material.hpp"
#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Transform.hpp"

namespace volt::gfx
{

    class RenderObject
    {
    private:
        Material  material;
        Mesh      mesh;
        Transform transform;

    public:
        RenderObject(Material mat, Mesh m, Transform t);
        RenderObject(const RenderObject &other);
        RenderObject &operator=(const RenderObject &other);
        RenderObject(RenderObject &&other);
        RenderObject &operator=(RenderObject &&other);
        ~RenderObject();

        void Bind() const;

        Material & GetMaterial();
        Mesh &     GetMesh();
        Transform &GetTransform();
    };

} // namespace volt::gfx
#endif
