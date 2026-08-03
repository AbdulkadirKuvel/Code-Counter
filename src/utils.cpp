#include <utils.hpp>

namespace fs = std::filesystem;

namespace utils
{
    std::string read_file_into_buffer(const fs::path &filepath, std::error_code &ec)
    {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file)
        {
            ec = std::make_error_code(std::errc::no_such_file_or_directory);
            return "";
        }

        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer;
        buffer.resize(size);

        if (file.read(buffer.data(), size))
        {
            return buffer;
        }

        ec = std::make_error_code(std::errc::io_error);
        return "";
    }
}