#include <iostream>
#include <string>
#include <listing.hpp>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string path;
    if (argc < 2)
    {
        std::cout << "Using current path as default\n";
        path = ".";
    }
    else
    {
        path = argv[1];
    }

    // std::cout << "--- list_dirs ---" << std::endl;
    // std::cout << "--- list_dirs_recursive ---" << std::endl;
    // std::cout << "--- list_files ---" << std::endl;
    // std::cout << "--- list_files_recursive ---" << std::endl;
    auto paths = list_dirs(path);
    // auto paths = list_dirs_recursive(path);
    // auto paths = list_files(path);
    // auto paths = list_files_recursive(path);
    for (size_t i = 0; i < paths.size(); i++) {
        std::cout << paths.at(i) << "\n"; 
    }
    
    return 0;
}