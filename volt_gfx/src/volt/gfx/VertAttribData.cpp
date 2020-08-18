#include "volt/gfx/VertAttribData.hpp"

using namespace volt::gfx;

VertAttribData::VertAttribData(std::string const &name, GLuint startIndex,
                               std::size_t stride, std::size_t offset,
                               bool instanced)
{
    this->name       = name;
    this->startIndex = startIndex;
    this->stride     = stride;
    this->offset     = offset;
    this->instanced  = instanced;
}