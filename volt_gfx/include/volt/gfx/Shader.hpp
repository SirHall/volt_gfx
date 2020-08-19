#pragma once
#ifndef shader_hpp
#define shader_hpp

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/ShaderSource.hpp"
#include "volt/gfx/VBO.hpp"
#include "volt/gfx/VertAttribData.hpp"

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace volt::gfx
{
#pragma region Uniform Assignment Specialization

    template <typename T>
    void AssignUniform(GLint uniformLoc, const T &value);

    template <>
    void AssignUniform(GLint uniformLoc, const GLuint &value);

    template <>
    void AssignUniform(GLint uniformLoc, const GLint &value);

    template <>
    void AssignUniform(GLint uniformLoc, const GLfloat &value);

    // template <>
    // void AssignUniform(GLint uniformLoc, const glm::vec1 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec2 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec3 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::vec4 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec1> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec2> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec3> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::vec4> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::quat &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x3 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x2 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat2x4 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x2 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat3x4 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const glm::mat4x3 &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x3> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x2> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat2x4> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x2> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat3x4> &value);

    template <>
    void AssignUniform(GLint uniformLoc, const std::vector<glm::mat4x3> &value);

#pragma endregion

    class Shader
    {
    private:
        std::shared_ptr<GLuint> program;

        static bool CompileShadelet(ShadeletSource const &    shadeletSource,
                                    GLuint &                  id,
                                    std::vector<std::string> &errors);
        Shader(GLuint programID);

    public:
        Shader(const Shader &other);
        Shader &operator=(const Shader &other);
        Shader(Shader &&other);
        Shader &operator=(Shader &&other);
        ~Shader();

        static Shader CompileShader(ShaderSource const &      source,
                                    std::vector<std::string> &errors);

        GLuint GetProgram() const;

        void Bind() const;

        bool GetUniformLocation(const std::string &uniformName,
                                GLint &            uniformLocOut) const;

        // template <typename T>
        // void SetUniform(GLint uniformLoc, const T &value);

        template <typename T>
        void SetUniform(GLint uniformLoc, const T &value)
        {
            this->Bind();
            AssignUniform<T>(uniformLoc, value);
        }

        bool IsValid() const;

        template <typename VertT, typename InstT>
        void SetVBODataTypes()
        {
            std::vector<VertAttribData> vboData;
            GLuint                      attribIndex = 0;
            attribIndex = GenVBO<VertT>(attribIndex, vboData);
            attribIndex = GenVBO<InstT>(attribIndex, vboData);

            for (auto const &attribData : vboData)
            {
                GLCall(glBindAttribLocation(*this->program,
                                            attribData.startIndex,
                                            attribData.name.c_str()));
            }
            GLCall(glLinkProgram(*this->program));
        }
    };
} // namespace volt::gfx
#endif
