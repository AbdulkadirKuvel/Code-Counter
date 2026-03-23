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
    void scan(types::Config);
    std::vector<fs::path> list_files(fs::path);
    std::vector<fs::path> list_files_recursive(fs::path);
}

#endif