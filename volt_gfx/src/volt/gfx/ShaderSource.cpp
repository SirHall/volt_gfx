#include "volt/gfx/ShaderSource.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace volt::gfx;

ShaderSource::ShaderSource(std::string name) { this->name = name; }

ShaderSource::~ShaderSource() {}

const std::string &ShaderSource::GetName() const { return this->name; }

void ShaderSource::AddShadelet(ShadeletSource shadelet)
{
    shadelets.push_back(shadelet);
}

std::vector<ShadeletSource> const &ShaderSource::GetShadelets() const
{
    return shadelets;
}