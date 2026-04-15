#include <parser.hpp>

types::Config parser(int argc, char *argv[])
{
    fs::path path;
    types::Config config;
    if (argc >= 2)
    {
        // bool in_flag = 0;

        bool config_file = true;
        std::string config_path;
        // First iterator: defines if user wants to get help, or use a prepared config file
        for (int i = 1; i < argc; i++)
        {
            std::string_view arg = argv[i];
            if (arg == "-h" || arg == "--help")
            {
                formatter::print_help();
                config.help_requested = true;
                return config;
            }

            if (config_file)
            {
                if (arg[0] != '-')
                { // CONFIG FILE NAME HAS BEEN DETECTED
                    config_path = arg;
                }
                else
                { // CONFIG FILE NAME HAS BEEN ASSIGNED TO DEFAULT
                    config_path = "config.json";
                }
            }
            else
            {

                if (arg == "-c" || arg == "--config")
                {
                    config_file = true;
                }
            }
        }

        while (config_file)
        {
            std::ifstream file(config_path, std::ios::in);
            if (!file.is_open())
            {
                std::println("File \"{}\" could not open for reading", config_path);
                break;
            }

            std::string line;
            while (std::getline(file, line))
            {
                for (char c : line)
                {
                    if (isspace(c))
                        continue;
                    else if (c == '#') // this is a comment
                    {
                        break;
                    }
                    else // this is not a comment
                    {
                        int index = line.find('=');
                        if (index)
                        { // key=value has been found
                            std::string_view key = line.substr(0, index);
                            std::string_view value = line.substr(index + 1);

                            bool val = tolower(value[0]) == 't';

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
                            else if (key == "blacklist") {
                                // config.blacklist = value; // TODO
                            }
                            else if (key == "whitelist") {
                                // config.whitelist = value // TODO
                            }
                        }
                        else // Not understandable
                        {
                        }
                    }
                }
            }

            file.close();
        }

        for (int i = 1; i < argc; i++)
        {
            std::string_view arg = argv[i];

            if (arg == "-h" || arg == "--help")
            {
                formatter::print_help();
                config.help_requested = true;
                return config;
            }
            else if (arg == "-r" || arg == "--recursive")
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
            else
            {
                config.path = arg;
            }
        }
    }
    return config;
}