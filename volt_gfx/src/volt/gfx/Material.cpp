#include "volt/gfx/Material.hpp"

using namespace volt::gfx;

Material::Material(Shader shader) : sh(shader)
{
    this->RetreiveDefaultUniforms();
}

Material::Material(const Material &other)
    : sh(other.sh), uniformTex(other.uniformTex)
{
    this->uniformProjection = other.uniformProjection;
    this->uniformView       = other.uniformView;
    this->uniformModel      = other.uniformModel;
    this->uniformPVM        = other.uniformPVM;
    this->uniformPV         = other.uniformPV;
}

Material &Material::operator=(const Material &other)
{
    this->sh                = other.sh;
    this->uniformProjection = other.uniformProjection;
    this->uniformView       = other.uniformView;
    this->uniformModel      = other.uniformModel;
    this->uniformPVM        = other.uniformPVM;
    this->uniformPV         = other.uniformPV;
    this->uniformTex        = other.uniformTex;
    return *this;
}

Material::Material(Material &&other)
    : sh(std::move(other.sh)), uniformTex(std::move(other.uniformTex))
{
    this->uniformProjection = other.uniformProjection;
    this->uniformView       = other.uniformView;
    this->uniformModel      = other.uniformModel;
    this->uniformPVM        = other.uniformPVM;
    this->uniformPV         = other.uniformPV;
}

Material &Material::operator=(Material &&other)
{
    this->sh                = std::move(other.sh);
    this->uniformProjection = other.uniformProjection;
    this->uniformView       = other.uniformView;
    this->uniformModel      = other.uniformModel;
    this->uniformPVM        = other.uniformPVM;
    this->uniformPV         = other.uniformPV;
    this->uniformTex        = std::move(other.uniformTex);
    return *this;
}

Material::~Material() {}

void Material::RetreiveDefaultUniforms()
{
    this->sh.GetUniformLocation("gfxProjection", this->uniformProjection);
    this->sh.GetUniformLocation("gfxView", this->uniformView);
    this->sh.GetUniformLocation("gfxModel", this->uniformModel);
    this->sh.GetUniformLocation("gfxPVM", this->uniformPVM);
    this->sh.GetUniformLocation("gfxPV", this->uniformPV);
    // Sampler2D arrays don't work very well in OpenGL, so it isn't supported
    // here for shaders
    for (int i = 0; i < this->uniformTex.size(); i++)
        this->sh.GetUniformLocation("gfxTex" + std::to_string(i),
                                    this->uniformTex[i]);
}

#pragma Default Uniforms

bool Material::HasUniformModel() { return this->uniformModel >= 0; }
bool Material::HasUniformView() { return this->uniformView >= 0; }
bool Material::HasUniformProjection() { return this->uniformProjection >= 0; }
bool Material::HasUniformPVM() { return this->uniformPVM >= 0; }
bool Material::HasUniformPV() { return this->uniformPV >= 0; }
bool Material::HasUniformTex(std::uint8_t imageUnit)
{
    return this->uniformTex[imageUnit] >= 0;
}

void Material::SetUniformPVM(glm::mat4 projection, glm::mat4 view,
                             glm::mat4 model)
{
    if (this->HasUniformProjection())
        this->sh.SetUniform(this->uniformProjection, projection);
    if (this->HasUniformView())
        this->sh.SetUniform(this->uniformView, view);
    if (this->HasUniformModel())
        this->sh.SetUniform(this->uniformModel, model);
    if (this->HasUniformPVM())
        this->sh.SetUniform(this->uniformPVM, projection * view * model);
    if (this->HasUniformPV())
        this->sh.SetUniform(this->uniformPV, projection * view);
}

void Material::SetUniformTex(Texture &tex, std::uint8_t imageUnit)
{
    if (HasUniformTex(imageUnit))
        this->sh.SetUniform(this->uniformTex[imageUnit], tex.GetTexID());
}

#pragma endregion

void Material::Bind() const { this->sh.Bind(); }