#include <scanner.hpp>
#include <string>
#include <unordered_set>
namespace scanner
{
    namespace internal
    {
        bool in_whitelist(const fs::path &path, const std::unordered_set<std::string> &whitelist)
        {
            const std::string ext = path.extension().string();
            if (!ext.empty() && whitelist.contains(ext))
                return true;

            const std::string filename = path.filename().string();
            return whitelist.contains(filename);
        }

        bool in_blacklist(const fs::path &path, const std::unordered_set<std::string> &blacklist)
        {
            const std::string filename = path.filename().string();
            const std::string ext = path.extension().string();

            return blacklist.contains(filename) || (!ext.empty() && blacklist.contains(ext));
        }
    }
    std::vector<fs::path> scan(types::Config config)
    {
        std::vector<fs::path> paths;
        if (config.recursive)
        {
            return list_files_recursive(config.path, config);
        }
        return list_files(config.path, config);
    }

    std::vector<fs::path> list_files(const fs::path &path, const types::Config &config)
    {
        std::vector<fs::path> paths;

        auto it = fs::directory_iterator(path);
        auto endit = fs::end(it);

        while (it != endit)
        {
            if (in_whitelist(it->path()))
            {
                paths.push_back(it->path().generic_string());
            }
            ++it;
        }

        return paths;
    }

    std::vector<fs::path> list_files_recursive(const fs::path &root, const types::Config &config)
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
