#include <parser.hpp>
#include <utils.hpp>
#include <print>
#include <format>
#include <ranges>

namespace parser_internal
{
    void parse_config_file(const fs::path &path, types::Config &config)
    {
        std::ifstream file(path, std::ios::in);
        if (!file.is_open())
        {
            config.error_requested = true;
            config.error_info.title = "File Could Not Open";
            config.error_info.message = std::format("File \"{}\" could not open for reading.", config.config_path.generic_string());
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::string_view line_view = utils::sv_trim(line);

            if (line_view.empty() || line_view.starts_with('#'))
                continue;

            size_t equal_index = line_view.find('=');
            if (equal_index == std::string_view::npos)
                continue;

            std::string_view key = utils::sv_trim(line_view.substr(0, equal_index));
            std::string_view value = utils::sv_trim(line_view.substr(equal_index + 1));

            if (value.empty()) // Change when value can be empty
            {
                config.error_requested = true;
                config.error_info.title = "Config File Missing Value";
                config.error_info.message = std::format("Config file has missing value\
                    \n\"{}\"\
                    value must not be empty.",
                                                        line_view);
            }

            bool bool_val = (!value.empty() && std::tolower(static_cast<unsigned char>(value[0])) == 't');

            if (key == "recursive")
                config.recursive = bool_val;
            else if (key == "json")
                config.json_form = bool_val;
            else if (key == "markdown")
                config.md_form = bool_val;
            else if (
                key == "blacklist" || key == "whitelist" ||
                key == "add blacklist" || key == "add whitelist" ||
                key == "remove blacklist" || key == "remove whitelist")
            {
                auto &target_set = (key.contains("black")) ? config.blacklist : config.whitelist;

                if (key == "blacklist" || key == "whitelist")
                {
                    target_set.clear();
                }

                for (auto &&chunk : value | std::views::split(','))
                {
                    std::string_view token{chunk.begin(), chunk.end()};
                    token = utils::sv_unquote(utils::sv_trim(token));

                    if (!token.empty())
                    {
                        if (key.starts_with('a')) // add or remove?
                            target_set.emplace(token);
                        else
                            target_set.erase(std::string(token));
                    }
                }
            }
        }
    }
}

types::Config parser(int argc, char *argv[])
{
    types::Config config;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            config.help_requested = true;
            return config;
        }
        else if (arg == "-r" || arg == "--recursive")
            config.recursive = true;

        else if (arg == "-m" || arg == "--markdown")
            config.md_form = true;

        else if (arg == "-j" || arg == "--json")
            config.json_form = true;

        else if (arg == "-c" || arg == "--config")
        {
            if (i + 1 >= argc || argv[i + 1][0] == '-')
            {
                config.error_requested = true;
                config.error_info.title = "Missing File Path Error";
                config.error_info.message = "-c/--config flag requires a file path.";
                return config;
            }

            fs::path config_path = argv[++i];
            parser_internal::parse_config_file(config_path, config);

            // TODO: parse_config_file may override terminal commands. Must not happen.
            
            if (config.error_requested)
                return config;
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (i + 1 >= argc || argv[i + 1][0] == '-')
            {
                config.error_requested = true;
                config.error_info.title = "Missing File Path Error";
                config.error_info.message = "-o/--output flag requires a file path.";
                return config;
            }

            config.output_path = argv[++i];
        }
        else if (!arg.starts_with('-'))
        {
            fs::path target(arg);
            if (fs::is_regular_file(target))
            {
                // This is just one file
                config.single_file = true;
            }
            else if (!fs::is_directory(target))
            {
                config.error_requested = true;
                config.error_info.title = "Not a Directory Error";
                config.error_info.message = std::format("Given path '{}' is not a directory.", target.generic_string());
                return config;
            }
            config.path = target;
        }
    }
    return config;
}