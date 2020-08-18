#pragma once
#ifndef VOLT_GFX_VERTATTRIBDATA_HPP
#define VOLT_GFX_VERTATTRIBDATA_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/VAOGen.hpp"

#include <functional>
#include <string>

namespace volt::gfx
{
    struct VertAttribData
    {
        VertAttribData(std::string const &name, GLuint startIndex,
                       std::size_t stride, std::size_t offset,
                       bool instanced = false);

        std::string name;
        GLuint      startIndex = 0;
        std::size_t stride     = 0;
        std::size_t offset     = 0;
        bool        instanced  = false;

        std::function<void(void)> generator;

        template <typename T>
        void SetGenerator()
        {
            this->generator = std::bind(
                [](std::string const &name, GLuint startIndex,
                   std::size_t stride, std::size_t offset, bool instanced) {
                    VAOGen::GenVertAttrib<T>(name, startIndex, stride, offset,
                                             instanced);
                },
                this->name, this->startIndex, this->stride, this->offset,
                this->instanced);
        }
    };
} // namespace volt::gfx
#endif
