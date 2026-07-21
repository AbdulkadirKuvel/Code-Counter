#include <parser.hpp>
#include <print>

types::Config parser(int argc, char *argv[])
{
    fs::path path;
    types::Config config;

    if (argc >= 2)
    {
        // bool in_flag = 0;

        // First iterator: defines if user wants to get help, or use a prepared config file
        bool config_use = false;
        for (int i = 1; i < argc; i++)
        {
            std::string_view arg = argv[i];
            // print the help and shut down the app
            if (arg == "-h" || arg == "--help")
            {
                config.help_requested = true;
                return config;
            }
            else if (arg == "-o" || arg == "--output")
            {
                if (i + 1 >= argc)
                {
                    config.error_requested = true;
                    config.error_info.title = "Not Enough Arguments";
                    config.error_info.message = "Output flag has been risen but no argument given afterwards.";
                    return config;
                }

                std::string_view next_arg = argv[i + 1];

                if (next_arg.starts_with('-')) // Next argument is a flag: Error
                {
                    config.error_requested = true;
                    config.error_info.title = "Missing Filename Error";
                    config.error_info.message = "Output flag has been risen but no path indicated.";
                    return config;
                }

                fs::path target(next_arg);

                if (fs::exists(target)) // TODO: user may want to overwrite?
                {
                    config.error_requested = true;
                    config.error_info.title = "Existing File Error";
                    config.error_info.message = "Given output path already exists.";
                    return config;
                }

                // Everything is A-okay.
                config.output_path = argv[i + 1];
                i++;
            }
            else if (arg == "-c" || arg == "--config") // CONFIG USE HAS BEEN DETECTED
            {
                config_use = true;
                if (i + 1 >= argc)
                {
                            config.error_requested = true;
                    config.error_info.title = "Not Enough Arguments";
                    config.error_info.message = "Config flag has been risen but no argument given afterwards.";
                            return config;
                        }

                std::string_view next_arg = argv[i + 1];

                if (next_arg.starts_with('-')) // Next argument is a flag: Error
                    {
                        config.error_requested = true;
                        config.error_info.title = "Missing Filename Error";
                        config.error_info.message = "Config flag has been risen but no file indicated.";
                        return config;
                    }

                fs::path target(next_arg);

                if (fs::is_directory(target)) // Given path is a folder: Error
                {
                    config.error_requested = true;
                    config.error_info.title = "Not a File Error";
                    config.error_info.message = std::format("Given path \"{}\" is a directory.", target.generic_string());
                    return config;
                }

                if (!fs::exists(target)) // Given path does not exist: Error
                {
                    config.error_requested = true;
                    config.error_info.title = "No File Found Error";
                    config.error_info.message = std::format("Given file \"{}\" could not be found.", target.generic_string());
                    return config;
                }

                if (!fs::is_regular_file(target)) // Given path is not a valid file (e.g. Device Files, sockets...): Error
                {
                        config.error_requested = true;
                    config.error_info.title = "False File Path Error";
                    config.error_info.message = std::format("Given path \"{}\" is not a valid config file.", target.generic_string());
                        return config;
                }

                // Everything is A-okay.
                config.config_path = argv[i + 1];
                // config_file_name_exists = true;
                i++; 
            }
            else if (!arg.starts_with('-')) // Handle if path is not a folder.
            {
                fs::path target(arg);

                if (!fs::is_directory(target)) {
                    config.error_requested = true;
                    config.error_info.title = "Not a Directory Error";
                    config.error_info.message = std::format("Given path \"{}\" is not a directory.", target.generic_string());
                    return config;
                }

                config.path = target;
            }
        }

        // Reading the config file
        if (config_use)
        {
            std::ifstream file(config.config_path, std::ios::in);
            if (!file.is_open())
            {
                config.error_requested = true;
                config.error_info.title = "File Could Not Open";
                config.error_info.message = std::format("File \"{}\" could not open for reading.", config.config_path.generic_string());
                return config;
            }

                std::string line;
                // Second iterator: reads the config file.
                while (std::getline(file, line))
                {
                    if (line == "")
                        continue;
                    else if (line[0] == '#') // comment
                    {
                    // ignore
                    }
                    else // must be code
                    {
                        // e.g: "recursive = true"
                        size_t index = line.find('=');
                        if (index != std::string::npos)
                        {
                        // DO THIS TO AVOID DANGLING VIEW.
                        std::string_view line_view = line;

                        std::string_view key = utils::trim(line_view.substr(0, index));    // e.g: "recursive"
                        std::string_view value = utils::trim(line_view.substr(index + 1)); // e.g: "true"

                        // Information: String.substr() does create a new string, then deletes it after.
                        // when value variable is created it is overwritten on key. To avoid this,
                        // an extra string_view named line_view was created above, because line_view.substr()
                        // does not create and delete another string like line.substr()

                            if (value.empty())
                            {
                            continue; // TODO: Throw error
                            }
                            bool val = tolower(value[0]) == 't'; // "true" or "false"

                            if (key == "recursive")
                            {
                                config.recursive = val;
                            }
                            else if (key == "text")
                            {
                            config.md_form = val;
                            }
                            else if (key == "json")
                            {
                            config.json_form = val;
                            }
                            else if (key == "blacklist")
                            {
                                config.blacklist.clear();

                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.blacklist.insert(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                config.blacklist.insert(std::string(value.substr(start)));
                            }
                            else if (key == "whitelist")
                            {
                                config.whitelist.clear();

                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.whitelist.insert(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                config.whitelist.insert(std::string(value.substr(start)));
                            }
                            else if (key == "add_blacklist")
                            {
                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.blacklist.insert(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                config.blacklist.insert(std::string(value.substr(start)));
                            }
                            else if (key == "add_whitelist")
                            {

                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.whitelist.insert(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                config.whitelist.insert(std::string(value.substr(start)));
                            }
                            else if (key == "remove_blacklist")
                            {
                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.blacklist.erase(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                config.blacklist.erase(std::string(value.substr(start)));
                            }
                            else if (key == "remove_whitelist")
                            {
                                size_t start = 0;
                                size_t end = value.find(',');

                                while (end != std::string::npos)
                                {
                                    std::string item = std::string(value.substr(start, end - start));
                                    config.whitelist.erase(item);

                                    start = end + 1;
                                    end = value.find(',', start);
                                }
                                // Do not miss the last word
                                config.whitelist.erase(std::string(value.substr(start)));
                            }
                            else
                            {
                                // none
                            }
                        }
                        else
                        {
                            // pass it, whatever it is.
                        }
                    }
                }
            }
            file.close();
        }

        // Third iterator: configuration from console (most authorization)
        for (int i = 1; i < argc; i++)
        {
            std::string_view arg = argv[i];

            // "-h" or "--help" will be fore-handled in first iterator
            if (arg == "-r" || arg == "--recursive")
            {
                config.recursive = true;
            }
            else if (arg == "-t" || arg == "--text")
            {
                config.output_text = true;
            }
            else if (arg == "-j" || arg == "--json")
            {
                config.output_json = true;
            }
            else if (arg == "-c" || arg == "--config")
            {
                if (!config_file_name_exists)
                    continue;
                else
                    i++;
            }
            else
            {
                config.path = arg;
            }
        }
    }
    return config;
}