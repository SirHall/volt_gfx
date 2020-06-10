#include "volt/gfx/ShadeletSource.hpp"

#include <filesystem>
#include <fstream>

using namespace volt::gfx;

ShadeletSource::ShadeletSource(std::string filePath, GLenum type)
    : path(filePath), tp(type)
{
    if (std::filesystem::exists(path))
    {
        auto shadeletSourceFile = std::ifstream(path);
        this->src =
            std::string(std::istreambuf_iterator<char>(shadeletSourceFile),
                        std::istreambuf_iterator<char>());
    }
    else
    {
        this->src = "";
    }
}

ShadeletSource::ShadeletSource(const ShadeletSource &other)
    : src(other.src), tp(other.tp), path(other.path)
{
}

ShadeletSource &ShadeletSource::operator=(const ShadeletSource &other)
{
    this->src  = other.src;
    this->tp   = other.tp;
    this->path = other.path;
    return *this;
}

ShadeletSource::ShadeletSource(ShadeletSource &&other)
    : src(std::move(other.src)), tp(other.tp), path(std::move(other.path))
{
}

ShadeletSource &ShadeletSource::operator=(ShadeletSource &&other)
{
    this->src  = std::move(other.src);
    this->tp   = other.tp;
    this->path = other.path;
    return *this;
}

ShadeletSource::~ShadeletSource() {}

std::string const &ShadeletSource::GetFullPath() const { return this->path; }

std::string const &ShadeletSource::GetSource() const { return this->src; }

GLenum ShadeletSource::GetType() const { return this->tp; }
