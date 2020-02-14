#include "volt/gfx/ShadeletSource.hpp"

#include <filesystem>
#include <fstream>

using namespace volt::gfx;

ShadeletSource::ShadeletSource(std::string src, GLenum tp, std::string path)
    : source(src), type(tp), fullPath(path)
{
}

ShadeletSource::~ShadeletSource() {}

const std::string &ShadeletSource::GetFullPath() const
{
    return this->fullPath;
}

const std::string &ShadeletSource::GetSource() const { return this->source; }

GLenum ShadeletSource::GetType() const { return this->type; }

ShadeletSource LoadFromFile(std::string filePath, GLenum type)
{
    auto shadeletSourceFile = std::ifstream(filePath);
    auto shadeletContents =
        std::string(std::istreambuf_iterator<char>(shadeletSourceFile),
                    std::istreambuf_iterator<char>());
    return ShadeletSource(shadeletContents, type, filePath);
}