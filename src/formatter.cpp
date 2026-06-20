#include <formatter.hpp>
#include <iostream>
#include <lexer.hpp>
#include <print>
#include <string_view>
#include <fstream>
#include <map>

constexpr std::string_view str = "│ {:<10} │ {:>7} │ {:>9} │ {:>9} │ {:>9} │ {:>9} │";

namespace formatter
{
    void print_files_stats(std::map<std::string, types::FileStats> gathered_stats)
    {
        constexpr std::string_view upmost_border = "┌────────────┬─────────┬───────────┬───────────┬───────────┬───────────┐";
        constexpr std::string_view middle_border = "├────────────┼─────────┼───────────┼───────────┼───────────┼───────────┤";
        constexpr std::string_view bottom_border = "└────────────┴─────────┴───────────┴───────────┴───────────┴───────────┘";

        std::println(upmost_border);
        std::println(str,
                     "Language", "Files", "Lines", "Code", "Comment", "Blank");
        std::println(middle_border);

        types::FileStats total;

        for (auto &[ext, stats] : gathered_stats)
        {
            total.file_count += stats.file_count;
            total.total_line += stats.total_line;
            total.comment_line += stats.comment_line;
            total.code_line += stats.code_line;
            total.blank_line += stats.blank_line;

            std::println(str,
                         ext, stats.file_count, stats.total_line,
                         stats.code_line, stats.comment_line, stats.blank_line);
        }

        std::println(str,
                     "Total", total.file_count, total.total_line,
                     total.code_line, total.comment_line, total.blank_line);

        std::println(bottom_border);
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
        std::println("│ -j │ --json        │ none         │ log into json file            │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -t │ --text        │ none         │ log into txt file             │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -c │ --config      │ <filepath>   │ use config file 'config.json' │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│    │               │              │                               │");
        std::println("├────┴───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│                    │      ──      │                               │");
        std::println("└────────────────────┴──────────────┴───────────────────────────────┘");
    }

    void print_error(types::Error error)
    {
        std::println("{}{}{}", color::bold_red, error.title, color::reset); // TODO: Maybe colorization afterwards?
        std::println("{}{}{}", color::red, error.message, color::reset);
    }

    // void log_files_stats(std::map<std::string, types::FileStats> gathered_stats, std::string name = "output.log")
    // {
    //     const char *upmost_border = "┌────────────┬─────────┬───────────┬───────────┬───────────┬───────────┐";
    //     const char *middle_border = "├────────────┼─────────┼───────────┼───────────┼───────────┼───────────┤";
    //     const char *bottom_border = "└────────────┴─────────┴───────────┴───────────┴───────────┴───────────┘";

    //     std::ofstream file(name, std::ios::app);

    //     /* Log it here */

    //     // file.write(upmost_border, );

    //     // std::println(upmost_border);
    //     // std::println(str,
    //     //              "Language", "Files", "Lines", "Code", "Comment", "Blank");
    //     // std::println(middle_border);

    //     types::FileStats total;

    //     for (auto &[ext, stats] : gathered_stats)
    //     {
    //         total.file_count += stats.file_count;
    //         total.total_line += stats.total_line;
    //         total.comment_line += stats.comment_line;
    //         total.code_line += stats.code_line;
    //         total.blank_line += stats.blank_line;

    //         // std::println(str,
    //         //              ext, stats.file_count, stats.total_line,
    //         //              stats.code_line, stats.comment_line, stats.blank_line);
    //     }

    //     file.close();

    //     // std::println(str,
    //     //              "Total", total.file_count, total.total_line,
    //     //              total.code_line, total.comment_line, total.blank_line);

    //     // std::println(bottom_border);
    // }
}
