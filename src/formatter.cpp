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


namespace formatter
{
    void report_files_stats(std::map<std::string, types::FileStats> stats, types::Config config)
    {
        std::unique_ptr<IReportFormatter> report_formatter;

        // Output form
        if (config.json_form)
        {
            report_formatter = std::make_unique<JsonFormatter>();
        }
        else if (config.md_form)
        {
            // report_formatter = std::make_unique<MarkdownFormatter>(); // TODO: Implement this.
        }
        else
        {
            report_formatter = std::make_unique<TableFormatter>();
        }

        // Output point
        if (!config.output_path.empty())
        {
            std::ofstream file(config.output_path);
            if (file.is_open()) {
                report_formatter->format(file, stats);
            }
            else {
                types::Error error;
                error.title = "File Error";
                error.message = "Could not create the output file.";
            }
            file.close();
        }
        else
        {
            report_formatter->format(std::cout, stats);
        }
    }

    void print_help()
    {
        std::println("┌───────────────────────────────────────────────────────────────────┐");
        std::println("│                type: tokenizer <path> [<commands>]                │"); // TODO: Change name
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
        std::println("│ -o │ --output      │ <outputpath> │ log into a file               │");
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
