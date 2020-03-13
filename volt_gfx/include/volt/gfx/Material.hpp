#pragma once
#ifndef VOLT_GFX_MATERIAL_HPP
#define VOLT_GFX_MATERIAL_HPP

#include "volt/gfx/Shader.hpp"
#include "volt/gfx/Texture.hpp"

namespace volt::gfx
{

    class Material
    {
    private:
        Shader sh;

        // Default uniforms
        GLint uniformProjection = -1, uniformView = -1, uniformModel = -1,
              uniformPVM = -1, uniformPV = -1;
        std::array<GLint, 16> uniformTex = std::array<GLint, 16>();

        void RetreiveDefaultUniforms();

    public:
        Material(Shader shader);
        Material(const Material &other);
        Material &operator=(const Material &other);
        Material(Material &&other);
        Material &operator=(Material &&other);
        ~Material();

        template <typename T>
        void SetUniform(GLint uniformLoc, const T &value)
        {
            sh.SetUniform<T>(uniformLoc, value);
        }

        bool HasUniformProjection();
        bool HasUniformView();
        bool HasUniformModel();
        bool HasUniformPVM();
        bool HasUniformPV();
        bool HasUniformTex(std::uint8_t imageUnit = 0);

        void SetUniformPVM(glm::mat4 projection, glm::mat4 view,
                           glm::mat4 model);
        void SetUniformTex(std::uint8_t imageUnit = 0);

        void Bind() const;

        Shader &GetShader() { return this->sh; }
    };

} // namespace volt::gfx
#endif
