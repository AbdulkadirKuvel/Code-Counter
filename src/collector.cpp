#include <scanner.hpp>
#include <parser.hpp>
#include <collector.hpp>

router get_analyzer(const std::string ext)
{
    // std::cout << "Debugging...\n";
    if (c_style.find(ext) != c_style.end())
    {
        // std::cout << "Debugging...\n";
        return file_analyzer_c;
    }
    if (py_style.find(ext) != py_style.end())
    {
        // std::cout << "Debugging...\n";
        return file_analyzer_py;
    }
    // if (py_style.find(ext) != py_style.end())
    //     return file_analyzer_py; // TODO
    // if (py_style.find(ext) != py_style.end())
    //     return file_analyzer_py; // TODO
    return nullptr;
}

void gather_file_stats(fs::path path) // Singular, outputs directly
{

    auto func = get_analyzer(path.extension().string());
    if (func)
    {
        FileStats file_stats = func(path);
        std::cout << "[0] --- File: " << path.filename() << "\n";
        std::cout << "blank lines   : " << file_stats.blank_line << "\n";
        std::cout << "comment lines : " << file_stats.comment_line << "\n";
        std::cout << "code lines    : " << file_stats.code_line << "\n";
        std::cout << "total         : " << file_stats.total_line << "\n";
    }
    else
    {
        std::cout << "[0] --- File: " << path.filename() << "\n";
        std::cout << "Undefined type: " << path.extension() << "\n";
    }
}

void gather_files_stats(fs::path path)
{
    auto files = list_files(path);

    for (const auto &file : files)
    {
        std::string ext = file.extension().string();
        auto func = get_analyzer(ext);
        if (func)
        {
            FileStats file_stats = func(file);
            FileStats &ex_stats = total_stats[ext];

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
}

void gather_recursive_file_stats(fs::path path)
{
    auto files = list_files_recursive(path);

    for (const auto &file : files)
    {
        std::string ext = file.extension().string();
        router func = get_analyzer(ext);
        if (func)
        {
            FileStats file_stats = func(file);
            FileStats &ex_stats = total_stats[ext];

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
}