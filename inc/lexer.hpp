#pragma once
#ifndef LEXER_HPP
#define LEXER_HPP

#include <filesystem>
#include <types.hpp>

namespace lexer
{
    enum DFA_Machine
    {
        IN_CODE, // start
        IN_SINGLE_STRING, // one character string (')
        IN_DOUBLE_STRING, // two characters string (")
        IN_MULTI_COMMENT, // in multiline comment (/*) or (""")
    };

    namespace fs = std::filesystem;

    types::FileStats file_analyzer_c(const fs::path &);
    types::FileStats file_analyzer_py(const fs::path &);
    types::FileStats file_analyzer_xml(const fs::path &);
}
#endif