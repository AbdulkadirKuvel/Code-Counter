#include <iostream>
#include <string>
#include <scanner.hpp>
#include <collector.hpp>
#include <formatter.hpp>
#include <filesystem>
#include <parser.hpp>

// namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    fs::path path;
    auto config = parser(argc, argv); // TODO

    if (config.help_requested)
    {
        formatter::print_help();
        exit(0);
    }
    else if (config.error_requested)
    {
        formatter::print_error(config.error_info);
        exit(0);
    }

    // std::cout << "[Devinfo]: after parser before scanner" << std::endl;
    auto paths = scanner::scan(config); // TODO
    // std::cout << "[Devinfo]: after scanner before collector" << std::endl;

    auto stats = collector::gather_files_stats(paths);
    formatter::print_files_stats(stats);

    // std::string str = "hello human!";

    // std::cout << "str        |" << str << "|" << std::endl;
    // std::cout << "str substr |" << str.substr(0, 5) << "|" << std::endl;
    // std::cout << "str substr |" << str.substr(6) << "|" << std::endl;

    return 0;
}