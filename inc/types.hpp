#pragma once

#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

namespace types
{
    struct Error {
        std::string title = "";        
        std::string message = "";
    };
    
    struct Config
    {
        // Initial Configuration
        bool help_requested = false;
        bool error_requested = false;
        bool recursive = false;
        bool output_json = false;
        bool output_text = false;
        bool output_console = true;

        std::unordered_set<std::string> whitelist = {
            ".py", ".java", ".c", ".cpp", ".h", ".hpp", ".cs",
            ".html", ".css", ".js", ".rb", ".go", ".asm", ".v",
            "makefile"};

        std::unordered_set<std::string> blacklist = {
            ".git", "bin", "obj", "lib", "node_modules", ".vs"};

        fs::path path = ".";
        // fs::path config_path = "config.txt";
        fs::path config_path;
        Error error_info;
    };

    struct FileStats
    {
        size_t file_count = 0;

        size_t code_line = 0;
        size_t comment_line = 0;
        size_t blank_line = 0;
        size_t total_line = 0;
    };
}