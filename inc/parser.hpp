#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <types.hpp>
#include <filesystem>
#include <formatter.hpp>
#include <fstream>
#include <print>
#include <string>

namespace fs = std::filesystem;

types::Config parser(int argc, char *argv[]);

#endif