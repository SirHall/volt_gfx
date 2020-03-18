#include "volt/gfx/Camera.hpp"

using namespace volt::gfx;

Camera::Camera() {}

Camera::Camera(const Camera &other) {}

Camera &Camera::operator=(const Camera &other) { return *this; }

Camera::Camera(Camera &&other) {}

Camera &Camera::operator=(Camera &&other) { return *this; }

Camera::~Camera() {}

Transform const &Camera::GetTransform() const { return this->transform; }
void             Camera::SetTransform(Transform v) { this->transform = v; }

glm::mat4 const &Camera::GetProjection() const
{
    if (this->projectionDirty)
    {
        this->projectionDirty = false;
        this->projection =
            this->isPerspective
                ? glm::perspective(this->perspectiveFov, this->ratio,
                                   this->nearPlane, this->farPlane)
                : glm::ortho(0.0f, this->ratio * this->orthoSize,
                             this->orthoSize, 0.0f, this->nearPlane,
                             this->farPlane);
    }
    return this->projection;
}

glm::mat4 Camera::GetPV() const
{
    return this->transform.GetMatrix() * this->GetProjection();
}

bool Camera::IsPerspective() const { return this->isPerspective; }
void Camera::SetPerspectiveMode(bool mode)
{
    this->isPerspective   = mode;
    this->projectionDirty = true;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->ratio           = aspectRatio;
    this->projectionDirty = true;
}

void Camera::SetOrthoSize(float size)
{
    this->orthoSize       = size;
    this->projectionDirty = true;
}
void Camera::SetPerspectiveFOV(float fov)
{
    this->perspectiveFov  = fov;
    this->projectionDirty = true;
}

void Camera::SetNearFarPlanes(float near, float far)
{
    // this->nearPlane       = near;
    // this->farPlane        = far;
    this->projectionDirty = true;
}