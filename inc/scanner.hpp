#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <filesystem>
#include <vector>
#include <types.hpp>

namespace fs = std::filesystem;

namespace scanner
{
    namespace internal
    {
        bool in_whitelist(const fs::path &, const std::unordered_set<std::string> &);
        bool in_blacklist(const fs::path &, const std::unordered_set<std::string> &);
    }
    std::vector<fs::path> scan(types::Config);
    std::vector<fs::path> list_files(const fs::path &, const types::Config &);
    std::vector<fs::path> list_files_recursive(const fs::path &, const types::Config &);
}

#endif