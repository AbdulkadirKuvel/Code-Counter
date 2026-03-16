#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void print_file_stats(fs::path);
void print_files_stats(fs::path);
void print_rec_files_stats(fs::path);

#endif