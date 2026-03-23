#include <collector.hpp>
#include <lexer.hpp>
#include <unordered_set>
#include <map>
#include <iostream>

namespace collector
{
    router get_analyzer(const std::string ext)
    {
        if (c_style.find(ext) != c_style.end())
        {
            return lexer::file_analyzer_c;
        }
        if (py_style.find(ext) != py_style.end())
        {
            return lexer::file_analyzer_py;
        }
        // if (py_style.find(ext) != py_style.end())
        //     return file_analyzer_py; // TODO: add support for other languages
        // if (py_style.find(ext) != py_style.end())
        //     return file_analyzer_py; // TODO
        return nullptr;
    }

    std::map<std::string, types::FileStats> gather_files_stats(std::vector<fs::path> files)
    {
        std::map<std::string, types::FileStats> gathered_stats;

        for (const auto &file : files)
        {
            std::string ext = file.extension().string();
            auto func = get_analyzer(ext);
            if (func)
            {
                types::FileStats file_stats = func(file);
                types::FileStats &ex_stats = gathered_stats[ext];

                ex_stats.blank_line += file_stats.blank_line;
                ex_stats.comment_line += file_stats.comment_line;
                ex_stats.code_line += file_stats.code_line;
                ex_stats.total_line += file_stats.total_line;
                ex_stats.file_count++;
            }
            else
            {
                std::cout << "Undefined type: " << file.extension() << "\n";
            }
        }
        return gathered_stats;
    }
}