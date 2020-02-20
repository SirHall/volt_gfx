#pragma once
#ifndef VOLT_GFX_RENDEROBJECT_HPP
#define VOLT_GFX_RENDEROBJECT_HPP

#include "volt/gfx/Material.hpp"
#include "volt/gfx/Mesh.hpp"
#include "volt/gfx/Texture.hpp"
#include "volt/gfx/Transform.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace volt::gfx
{

    class RenderObject
    {
    private:
        Material  material;
        Mesh      mesh;
        Transform transform;

        std::array<std::optional<Texture>, 16> texUnits;

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

        void SetTexture(Texture tex, std::uint8_t texUnit);
        void ClearTexture(std::uint8_t texUnit);
        bool HasTexture(std::uint8_t texUnit);
    };

} // namespace volt::gfx
#endif
