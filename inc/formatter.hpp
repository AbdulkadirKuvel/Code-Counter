#pragma once
#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <filesystem>
#include <types.hpp>
#include <map>

namespace fs = std::filesystem;

#include <string_view>

namespace color {
    // Rengi sıfırlamak için (Normal terminal rengine dönüş)
    constexpr std::string_view reset   = "\033[0m";

    // Temel Renkler (Normal)
    constexpr std::string_view red     = "\033[31m";
    constexpr std::string_view green   = "\033[32m";
    constexpr std::string_view yellow  = "\033[33m";
    constexpr std::string_view blue    = "\033[34m";
    constexpr std::string_view magenta = "\033[35m";
    constexpr std::string_view cyan    = "\033[36m";
    constexpr std::string_view white   = "\033[37m";

    // Kalın (Bold) Renkler - Hatalar ve başlıklar için daha okunaklı olur
    constexpr std::string_view bold_red     = "\033[1;31m";
    constexpr std::string_view bold_green   = "\033[1;32m";
    constexpr std::string_view bold_yellow  = "\033[1;33m";
    constexpr std::string_view bold_cyan    = "\033[1;36m";
}

namespace formatter
{
    void print_files_stats(std::map<std::string, types::FileStats>);
    void log_files_stats(std::map<std::string, types::FileStats>, std::string);
    void print_help();
    void print_error(types::Error);
}

#endif