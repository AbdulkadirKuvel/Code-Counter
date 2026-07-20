#include <formatter.hpp>
#include <iostream>
#include <lexer.hpp>
#include <print>
#include <string_view>
#include <fstream>
#include <map>

#include <IReportFormatter.hpp>
#include <JsonFormatter.hpp>
#include <TableFormatter.hpp>


constexpr std::string_view str = "│ {:<10} │ {:>7} │ {:>9} │ {:>9} │ {:>9} │ {:>9} │";

namespace formatter
{
    void report_files_stats(std::map<std::string, types::FileStats> stats, types::Config config)
    {

    }

    void print_help()
    {
        std::println("┌───────────────────────────────────────────────────────────────────┐");
        std::println("│                type: tokenizer <path> [<commands>]                │");
        std::println("├────────────────────┬──────────────┬───────────────────────────────┤");
        std::println("│      commands      │   argument   │          explanation          │");
        std::println("├────┬───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -h │ --help        │ none         │ show help                     │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -r │ --recursive   │ none         │ recursive search              │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -j │ --json        │ none         │ format stats as json          │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -m │ --markdown    │ none         │ format stats as markdown      │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -c │ --config      │ <filepath>   │ use config file 'config.json' │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -o │ --output      │ <path>       │ log into a file               │");
        std::println("├────┴───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│                    │      ──      │                               │");
        std::println("└────────────────────┴──────────────┴───────────────────────────────┘");
    }

    void print_error(types::Error error)
    {
        std::println("{}{}{}", color::bold_red, error.title, color::reset);
        std::println("{}{}{}", color::red, error.message, color::reset);
    }

    void print_content_info(std::string message)
    {
        std::println("{}{}{}", color::yellow, message, color::reset);
    }
}
