#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

bool in_whitelist(fs::path);
bool in_blacklist(fs::path);
std::vector<fs::path> list_dirs(fs::path);
std::vector<fs::path> list_dirs_recursive(fs::path);
std::vector<fs::path> list_files(fs::path);
std::vector<fs::path> list_files_recursive(fs::path);

#endif