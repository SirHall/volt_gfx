#pragma once
#ifndef VOLT_GFX_VBO_HPP
#define VOLT_GFX_VBO_HPP

#include "volt/gfx/Buffer.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"
#include "volt/gfx/VertAttribData.hpp"

#include <stdexcept>
#include <vector>

namespace volt::gfx
{
    template <typename T>
    GLuint GenVBO(GLuint attribIndex, std::vector<VertAttribData> &verts);

    template <typename T, GLenum VBOUsage>
    class VBO : public Buffer<T, GL_ARRAY_BUFFER, VBOUsage>
    {
    private:
        GLuint initAttribIndex;

    public:
        // TODO: Only allow this class to be instantiated withint volt_gfx
        VBO(GLuint vaoID, GLuint &attribIndex)
            : Buffer(vaoID), initAttribIndex(attribIndex)
        {
            this->Bind();
            auto vboData = std::vector<VertAttribData>();
            attribIndex  = GenVBO<T>(attribIndex, vboData);
            for (auto const &attribData : vboData)
                attribData.generator();
        }

        VBO(const VBO &other) = delete;
        //     : Buffer(other), initAttribIndex(other.initAttribIndex)
        // {
        //     this->Bind();
        //     GenVBO<T>(this->initAttribIndex);
        // }

        VBO &operator=(const VBO &other) = delete;
        // {
        //     Buffer::operator      =(other);
        //     this->initAttribIndex = other.initAttribIndex;
        //     this->Bind();
        //     GenVBO<T>(this->initAttribIndex);
        //     return *this;
        // }

        VBO(VBO &&other)
            : initAttribIndex(other.initAttribIndex),
              Buffer<T, GL_ARRAY_BUFFER, VBOUsage>(std::move(other))
        {
        }

        VBO &operator=(VBO &&other)
        {
            this->initAttribIndex = other.initAttribIndex;

            Buffer<T, GL_ARRAY_BUFFER, VBOUsage>::operator=(std::move(other));
            return *this;
        }

        ~VBO() {}
    };
} // namespace volt::gfx
#endif
