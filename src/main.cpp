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

    auto paths = scanner::scan(config); // TODO

    auto stats = collector::gather_files_stats(paths);

    formatter::report_files_stats(stats, config);

    return 0;
}