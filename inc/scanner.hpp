#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <filesystem>
#include <vector>
#include <types.hpp>

namespace fs = std::filesystem;

bool in_whitelist(fs::path);
bool in_blacklist(fs::path);
namespace scanner
{
}

#endif