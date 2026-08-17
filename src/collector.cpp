#include <collector.hpp>
#include <lexer.hpp>
#include <unordered_set>
#include <map>
#include <iostream>
#include <print>

namespace collector
{
    router get_analyzer(const std::string ext)
    {
        if (c_style.contains(ext))
            return lexer::file_analyzer_c;

        if (py_style.contains(ext))
            return lexer::file_analyzer_py;

        if (xml_style.contains(ext))
            return lexer::file_analyzer_xml;

        return nullptr;
    }

    std::map<std::string, types::FileStats> gather_files_stats(std::vector<fs::path> files)
    {
        std::map<std::string, types::FileStats> gathered_stats;

        for (const auto &file : files)
        {
            std::string ext = file.extension().string();
            types::FileStats file_stats;

            auto func = get_analyzer(ext);
            if (func)
            {
                std::error_code ec;
                std::string file_content = utils::read_file_into_buffer(file, ec);

                if (ec)
                {
                    std::println("File \"{}\" could not open for reading.", file.generic_string());
                    continue;
                }

                func(file_content, file_stats);

                types::FileStats &ex_stats = gathered_stats[ext];

                ex_stats.blank_line += file_stats.blank_line;
                ex_stats.comment_line += file_stats.comment_line;
                ex_stats.code_line += file_stats.code_line;
                ex_stats.total_line += file_stats.total_line;
                ex_stats.file_count++;
            }
            else
            {
                continue;
                // std::println("[Devinfo]: The type \"{}\" is not yet implemented.", file.extension().generic_string());
            }
        }
        return gathered_stats;
    }
}