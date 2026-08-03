#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string_view>
#include <filesystem>
#include <algorithm>
#include <format>
#include <ranges>
#include <fstream>

namespace fs = std::filesystem;

namespace utils
{
    inline std::string_view sv_trim(std::string_view sv)
    {
        sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
        sv.remove_suffix(sv.size() - std::min(sv.find_last_not_of(" \t\r\n") + 1, sv.size()));

        return sv;
    }

    inline std::string_view sv_unquote(std::string_view sv)
    {
        if (sv.size() >= 2 && ((sv.front() == '"' && sv.back() == '"') ||
                               (sv.front() == '\'' && sv.back() == '\'')))
        {
            return sv.substr(1, sv.size() - 2);
        }
        return sv;
    }

    std::string read_file_into_buffer(const fs::path &, std::error_code &);
}

#endif