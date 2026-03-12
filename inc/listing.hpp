#ifndef LISTING_HPP
#define LISTING_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

bool in_whitelist(fs::path);
bool in_blacklist(fs::path);
std::vector<fs::path> list_dirs(std::string);
std::vector<fs::path> list_dirs_recursive(std::string);
std::vector<fs::path> list_files(std::string);
std::vector<fs::path> list_files_recursive(std::string);

#endif