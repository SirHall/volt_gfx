#pragma once
#ifndef VOLT_GFX_TRANSFORM_HPP
#define VOLT_GFX_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace volt::gfx
{
    class Transform
    {
    private:
        glm::mat4 transform = glm::mat4(1.0f);

    public:
        Transform();
        Transform(glm::mat4 initTransform);
        Transform(glm::vec3 pos);
        Transform(const Transform &other);
        // Transform(Transform &&other);
        Transform &operator=(const Transform &other);
        // Transform &operator=(Transform &&other);
        ~Transform();

        glm::vec3 GetPosition() const;
        glm::quat GetRotation() const;

        void RotateLocal(glm::mat4 rotation);
        void RotateGlobal(glm::mat4 rotation);

        void Translate(glm::vec3 translation);

        const glm::f32 *GetValuePtr() const;

        glm::mat4 const &GetMatrix() const;
        void             SetMatrix(glm::mat4 m);

    }; // namespace volt::gfxclassTransform
} // namespace volt::gfx
#endif
