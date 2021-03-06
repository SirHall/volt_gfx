#pragma once
#ifndef VOLT_GFX_MESHINSTANCEDATA_HPP
#define VOLT_GFX_MESHINSTANCEDATA_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/VBO.hpp"

#include <glm/glm.hpp>

namespace volt::gfx
{
    struct MeshInstanceData
    {
    public:
        MeshInstanceData();
        MeshInstanceData(glm::mat4 transformMat);
        MeshInstanceData(const MeshInstanceData &other);
        MeshInstanceData &operator=(const MeshInstanceData &other);
        ~MeshInstanceData();

        glm::mat4 transform;
    };

    template <>
    GLuint GenVBO<MeshInstanceData>(GLuint                       attribIndex,
                                    std::vector<VertAttribData> &verts);
} // namespace volt::gfx
#endif
