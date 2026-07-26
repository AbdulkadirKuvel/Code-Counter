#include <string_view>
#include <algorithm>
#include <format>
#include <ranges>

namespace utils
{
    // std::string_view trim(std::string_view str)
    // {
    //     const char *whitespace_and_quotes = " \t\r\n\"";

    //     size_t first = str.find_first_not_of(whitespace_and_quotes);
    //     if (first == std::string_view::npos)
    //         return "";

    //     size_t last = str.find_last_not_of(whitespace_and_quotes);
    //     return str.substr(first, (last - first + 1));
    // }

    // constexpr std::string_view unquote_and_trim(std::string_view sv) noexcept {
    //     sv = trim(sv);
    //     if (sv.size() > 2 && ((sv.front() == '"' && sv.back() == '"') || (sv.front() == '\'' && sv.back() == '\'')))
    //     {
    //         sv.remove_prefix(1);
    //         sv.remove_suffix(1);
    //     }
    //     return trim(sv);
    // }

    constexpr std::string_view sv_trim(std::string_view sv)
    {
        sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
        sv.remove_suffix(sv.size() - std::min(sv.find_last_not_of(" \t\r\n") + 1, sv.size()));

        return sv;
    }

    constexpr std::string_view sv_unquote(std::string_view sv)
    {
        if (sv.size() >= 2 && (sv.front() == '"' && sv.back() == '"') ||
            sv.front() == '\'' && sv.back() == '\'')
        {
            return sv.substr(1, sv.size() - 2);
        }
        return sv;
    }

}