#pragma once
#ifndef VOLT_GFX_CAMERA_HPP
#define VOLT_GFX_CAMERA_HPP

#include <glm/glm.hpp>

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Transform.hpp"

namespace volt::gfx
{

    class Camera
    {
    protected:
        Transform transform;

        mutable bool      projectionDirty = true;
        mutable glm::mat4 projection      = glm::mat4(1.0f);
        bool              isPerspective   = true;
        float             ratio           = 1.0f;
        float             orthoSize       = 1.0f;
        float             perspectiveFov  = 60.0f;
        float             nearPlane       = 0.1f;
        float             farPlane        = 100.0f;

    public:
        Camera();
        Camera(const Camera &other);
        Camera &operator=(const Camera &other);
        Camera(Camera &&other);
        Camera &operator=(Camera &&other);
        ~Camera();

        Transform const &GetTransform() const;
        Transform &      GetTransform();
        void             SetTransform(Transform v);

        glm::mat4 const &GetProjection() const;

        glm::mat4 GetPV() const;

        bool IsPerspective() const;
        void SetPerspectiveMode(bool mode = true);

        void SetAspectRatio(float aspectRatio);
        void SetOrthoSize(float size);
        void SetPerspectiveFOV(float fov);

        void SetNearFarPlanes(float near, float far);
    };

} // namespace volt::gfx
#endif
