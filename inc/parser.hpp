#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <types.hpp>
#include <formatter.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

namespace parser_internal
{
    void parse_config_file(const fs::path &, types::Config &);
}

types::Config parser(int argc, char *argv[]);

#endif