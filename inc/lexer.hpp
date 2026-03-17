#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

struct FileStats
{
    int file_count = 0;
    
    size_t code_line = 0;
    size_t comment_line = 0;
    size_t blank_line = 0;
    size_t total_line = 0;
};

enum DFA_Machine
{
    State0, // start
    State1, // one character string (')
    State2, // two characters string (")
    State3, // in multiline comment (/*) or (""")
};

namespace fs = std::filesystem;

FileStats file_analyzer_c(fs::path);
FileStats file_analyzer_py(fs::path);

#endif