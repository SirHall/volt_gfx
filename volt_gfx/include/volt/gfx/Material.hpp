#pragma once
#ifndef VOLT_GFX_MATERIAL_HPP
#define VOLT_GFX_MATERIAL_HPP

#include "volt/gfx/Shader.hpp"

class Material
{
private:
public:
    Material();
    Material(const Material &other);
    Material(Material &&other);
    Material &operator=(const Material &other);
    Material &operator=(Material &&other);
    ~Material();
};

#endif
