#include <iostream>
#include <string>
#include <scanner.hpp>
#include <collector.hpp>
#include <formatter.hpp>
#include <filesystem>
#include <parser.hpp>
#include <IReportFormatter.hpp>
#include <TableFormatter.hpp>
#include <JsonFormatter.hpp> // TODO

// namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    fs::path path;
    auto config = parser(argc, argv);

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

    formatter::print_info("Completed reading configuration.");

    std::vector<std::filesystem::path> paths;

    paths = scanner::scan(config);

    formatter::print_info("Completed finding files.");

    auto stats = collector::gather_files_stats(paths);

    formatter::report_files_stats(stats, config);

    return 0;
}