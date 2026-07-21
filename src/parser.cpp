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
            else if (arg == "-c" || arg == "--config") // CONFIG USE HAS BEEN DETECTED
            {
                config_use = true;
                if (i + 1 < argc)
                {
                    if (argv[i + 1][0] != '-') // CONFIG FILE NAME HAS BEEN CONFIGURED
                    {
                        fs::path target(argv[i + 1]);

                        if (fs::is_regular_file(target)) // Given path is a filename
                        {
                            config.config_path = argv[i + 1];
                            config_file_name_exists = true;
                            i++;
                        }
                        else if (fs::is_directory(target)) // Given path is not a folder: Error
                        {
                            // This is a path, error
                            config.error_requested = true;
                            config.error_info.title = "Not a File Error";
                            config.error_info.message = "Given path \"", target, "\" is a directory.";
                            return config;
                        }
                        else // Given argument neither a file or folder: Error
                        {
                            config.error_requested = true;
                            config.error_info.title = "False File Path Error";
                            config.error_info.message = "Given argument \"", argv[i + 1], "\" is not a config file.";
                            return config;
                        }
                    }
                    else // Config filename has been forgotten: Error
                    {
                        config.error_requested = true;
                        config.error_info.title = "Missing Filename Error";
                        config.error_info.message = "Config flag has been risen but no file indicated.";
                        return config;
                    }
                }
                else {
                        config.error_requested = true;
                        config.error_info.title = "Missing Filename Error";
                        config.error_info.message = "Config flag has been risen but no file indicated.";
                        return config;

                }
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
                config.error_info.message = "File \"" + config.config_path.generic_string() + "\" could not open for reading.";
                return config;
            }
            else
            {
                std::string line;
                // Second iterator: reads the config file.
                while (std::getline(file, line))
                {
                    if (line == "")
                        continue;
                    else if (line[0] == '#') // comment
                    {
                    }
                    else // must be code
                    {
                        // e.g: "recursive = true"
                        size_t index = line.find('=');
                        if (index != std::string::npos)
                        {
                            std::string_view key = line.substr(0, index); // e.g: "recursive"
                            std::string_view value = line.substr(index + 1); // e.g: "true"
                            if (value.empty())
                            {
                                continue; // ADD FEATURE: Throw error
                            }
                            bool val = tolower(value[0]) == 't'; // "true" or "false"

                            if (key == "recursive")
                            {
                                config.recursive = val;
                            }
                            else if (key == "text")
                            {
                                config.output_text = val;
                            }
                            else if (key == "json")
                            {
                                config.output_json = val;
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