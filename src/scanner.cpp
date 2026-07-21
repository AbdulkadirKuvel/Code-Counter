#include <scanner.hpp>
#include <string>
#include <unordered_set>

std::unordered_set<std::string> blacklist;
std::unordered_set<std::string> whitelist;

bool in_whitelist(fs::path path)
{
    if (whitelist.find(path.extension().string()) != whitelist.end())
        return true;

    return false;
}

bool in_blacklist(fs::path path)
{
    if (blacklist.find(path.extension().string()) != blacklist.end())
        return true;

    return false;
}

namespace scanner
{
    std::vector<fs::path> scan(types::Config config)
    {
        whitelist = config.whitelist;
        blacklist = config.blacklist;
        std::vector<fs::path> paths;
        if (config.recursive)
        {
            paths = list_files_recursive(config.path);
        }
        else
        {
            paths = list_files(config.path);
        }
        return paths;
    }

    std::vector<fs::path> list_files(fs::path path)
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

    std::vector<fs::path> list_files_recursive(fs::path path)
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
}
