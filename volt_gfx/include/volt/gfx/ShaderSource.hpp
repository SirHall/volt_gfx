#pragma once
#ifndef ShaderSource_hpp
#define ShaderSource_hpp

#include "ShadeletSource.hpp"

#include <string>
#include <vector>

#include "volt/gfx/GLImport.hpp"

namespace volt::gfx
{

    class ShaderSource
    {
    private:
        std::string                 name;
        std::vector<ShadeletSource> shadelets;

    public:
        ShaderSource(std::string name);
        ~ShaderSource();

        const std::string &GetName() const;

        void AddShadelet(ShadeletSource shadelet);

        std::vector<ShadeletSource> const &GetShadelets() const;
    };
} // namespace volt::gfx
#endif
