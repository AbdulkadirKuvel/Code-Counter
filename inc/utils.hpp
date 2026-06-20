#include <string_view>

namespace utils
{
    // Başındaki ve sonundaki boşlukları, satır sonlarını ve tırnakları temizler
    std::string_view trim(std::string_view str)
    {
        // Silinmesini istediğimiz görünmez karakterler ve tırnak işareti
        const char *whitespace_and_quotes = " \t\r\n\"";

        size_t first = str.find_first_not_of(whitespace_and_quotes);
        if (first == std::string_view::npos)
            return ""; // Sadece boşluktan oluşuyorsa

        size_t last = str.find_last_not_of(whitespace_and_quotes);
        return str.substr(first, (last - first + 1));
    }
}