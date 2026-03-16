#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <filesystem>
#include <scanner.hpp>
#include <parser.hpp>
#include <iostream>
#include <unordered_set>
#include <map>

namespace fs = std::filesystem;

const std::unordered_set<std::string> c_style = {
    ".c",
    ".cpp",
    ".cxx",
    ".cc",
    ".h",
    ".hpp",
    ".cs",
    ".java",
    ".js",
    ".jsx",
    ".ts",
    ".tsx",
    ".go",
    ".rs",
    ".swift",
    ".kt",
};

const std::unordered_set<std::string> py_style = {
    ".py", ".pyx", ".pyw"};

const std::unordered_set<std::string> asm_style = {
    ".asm", ".s", ".lisp"};

const std::unordered_set<std::string> sql_style = {
    ".lua", ".sql"};

const std::unordered_set<std::string> shell_style = {
    "makefile", ".yml", ".sh"};

const std::unordered_set<std::string> xml_style = {
    ".xml", ".html", ".svg"};

const std::string ruby = ".rb";

std::map<std::string, FileStats> total_stats;

using router = FileStats(*)(fs::path);

router get_analyzer(const std::string);
void gather_file_stats(fs::path);
void gather_files_stats(fs::path);
void gather_recursive_file_stats(fs::path);

#endif