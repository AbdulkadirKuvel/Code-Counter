#include <string_view>
#include <algorithm>
#include <format>
#include <ranges>

namespace utils
{
    constexpr std::string_view sv_trim(std::string_view sv)
    {
        sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
        sv.remove_suffix(sv.size() - std::min(sv.find_last_not_of(" \t\r\n") + 1, sv.size()));

        return sv;
    }

    constexpr std::string_view sv_unquote(std::string_view sv)
    {
        if (sv.size() >= 2 && ((sv.front() == '"' && sv.back() == '"') ||
            (sv.front() == '\'' && sv.back() == '\'')))
        {
            return sv.substr(1, sv.size() - 2);
        }
        return sv;
    }

}