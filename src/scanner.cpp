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
        std::error_code ec;

        auto options = fs::directory_options::skip_permission_denied;
        auto it = fs::directory_iterator(path, options, ec);
        auto endit = fs::end(it);

        while (it != endit)
        {
            if (ec)
            {
                it.increment(ec);
                continue;
            }

            const auto &current_path = it->path();
            const auto ext = current_path.extension();
            const auto filename = current_path.filename();

            if (it->is_regular_file(ec))
            {

                if (!internal::in_blacklist(filename, config.blacklist) && (internal::in_whitelist(ext, config.whitelist) || internal::in_whitelist(filename, config.whitelist)))
                {
                    paths.push_back(current_path);
                }
            }
            it.increment(ec);
        }
        return paths;
    }

    std::vector<fs::path> list_files_recursive(const fs::path &root, const types::Config &config)
    {
        std::vector<fs::path> paths;
        std::error_code ec;

        auto options = fs::directory_options::skip_permission_denied;
        auto it = fs::recursive_directory_iterator(root, options, ec);
        auto endit = fs::end(it);

        while (it != endit)
        {
            if (ec)
            {
                it.increment(ec);
                continue;
            }

            const auto &current_path = it->path();
            const auto ext = current_path.extension();
            const auto filename = current_path.filename();

            if (it->is_directory(ec))
            {
                if (internal::in_blacklist(filename, config.blacklist) || internal::in_blacklist(current_path, config.blacklist))
                {
                    it.disable_recursion_pending();
                }
            }
            else if (it->is_regular_file(ec))
            {
                if (!internal::in_blacklist(filename, config.blacklist) && (internal::in_whitelist(ext, config.whitelist) || internal::in_whitelist(filename, config.whitelist)))
                {
                    paths.push_back(current_path);
                }
            }
            it.increment(ec);
        }
        return paths;
    }
}
