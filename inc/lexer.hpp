#pragma once
#ifndef LEXER_HPP
#define LEXER_HPP

#include <filesystem>
#include <types.hpp>

namespace lexer
{
    enum DFA_Machine
    {
        State0, // start
        State1, // one character string (')
        State2, // two characters string (")
        State3, // in multiline comment (/*) or (""")
    };

    namespace fs = std::filesystem;

    types::FileStats file_analyzer_c(fs::path);
    types::FileStats file_analyzer_py(fs::path);
}
#endif