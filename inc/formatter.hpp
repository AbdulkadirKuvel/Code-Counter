#pragma once
#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <filesystem>
#include <types.hpp>
#include <map>

namespace fs = std::filesystem;

namespace formatter
{
    void print_files_stats(std::map<std::string, types::FileStats>);
    void log_files_stats(std::map<std::string, types::FileStats>, std::string);
    void print_help();
}

#endif