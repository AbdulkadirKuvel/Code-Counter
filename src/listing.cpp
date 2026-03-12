#include <listing.hpp>

std::vector<std::string> blacklist = {
    ".git", "bin", "obj", "lib", "node_modules", ".vs"};

std::vector<std::string> whitelist = {
    ".py", ".java", ".c", ".cpp", ".h", ".hpp", ".cs",
    ".html", ".css", ".js", ".rb", ".go", ".asm", ".v"};

bool in_whitelist(fs::path path)
{
    for (size_t i = 0; i < whitelist.size(); i++)
    {
        if (whitelist.at(i) == path.extension())
        {
            return true;
        }
    }
    return false;
}

bool in_blacklist(fs::path path)
{
    for (size_t i = 0; i < blacklist.size(); i++)
    {
        if (blacklist.at(i) == path.filename())
        {
            return true;
        }
    }
    return false;
}

std::vector<fs::path> list_dirs(std::string path)
{
    std::vector<fs::path> paths;
    
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            paths.push_back(entry.path().generic_string());
        }
    }
    return paths;
}

std::vector<fs::path> list_dirs_recursive(std::string path)
{
    std::vector<fs::path> paths;
    
    auto it = fs::recursive_directory_iterator(path);
    auto endit = fs::end(it);

    while (it != endit)
    {
        if (it->is_directory())
        {
            if (in_blacklist(it->path()))
                it.disable_recursion_pending();
            else
                paths.push_back(it->path().generic_string());
        }
        ++it;
    }
    return paths;
}

std::vector<fs::path> list_files(std::string path)
{
    std::vector<fs::path> paths;
    
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            paths.push_back(entry.path().generic_string());
        }
    }
    
    return paths;
}

std::vector<fs::path> list_files_recursive(std::string path)
{
    std::vector<fs::path> paths;
    
    auto it = fs::recursive_directory_iterator(path);
    auto endit = fs::end(it);

    while (it != endit)
    {
        if (it->is_directory())
        {
            if (in_blacklist(it->path()))
                it.disable_recursion_pending();
        }
        else if (it->is_regular_file())
        { // when it is a file
            if (in_whitelist(it->path()))
                paths.push_back(it->path().generic_string());
        }
        ++it;
    }

    return paths;
}
