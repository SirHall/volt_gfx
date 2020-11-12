#pragma once
#ifndef MINE_GEN_HPP
#define MINE_GEN_HPP

#include <cstdint>
#include <vector>

enum Type
{
    Wall   = 0x0,
    Dugout = 0x1
};

std::vector<std::uint8_t> GenImg(std::vector<Type> const &types);

std::vector<Type> GenMine();

#endif
