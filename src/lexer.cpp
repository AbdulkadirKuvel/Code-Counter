#include <lexer.hpp>
#include <iostream>
#include <string>
#include <fstream>

namespace lexer
{

    /**
     * @brief for analyzing files that has comments like C
     * @param path path of the file
     * @returns types::FileStats
     */
    types::FileStats file_analyzer_c(fs::path path)
    {
        types::FileStats file_stats;
        DFA_Machine current_state = State0;

        bool in_multi_line_comment = 0;
        std::string line;
        std::ifstream file(path, std::ios::in);

        if (!file.is_open())
        {
            std::cerr << "File '" << path << "' could not open for reading.\n";
        }

        while (std::getline(file, line))
        {
            bool is_blank = true;
            for (char c : line)
            {
                if (!std::isspace(c))
                {
                    is_blank = false;
                    break;
                }
            }
            if (is_blank)
            {
                file_stats.blank_line++;
                file_stats.total_line++;
                continue;
            }

            bool has_code = 0;
            bool has_comment = 0;
            if (in_multi_line_comment)
                current_state = State3;

            for (size_t i = 0; i < line.length(); i++)
            {
                switch (current_state)
                {
                case State0:
                {
                    if (line[i] == '/')
                    {
                        if (i + 1 < line.length())
                        {
                            if (line[i + 1] == '/')
                            {
                                has_comment = true;
                                i = line.length();
                            }
                            else if (line[i + 1] == '*')
                            {
                                i++;
                                in_multi_line_comment = 1;
                                has_comment = true;
                                current_state = State3;
                            }
                        }
                    }
                    else if (line[i] == '\'')
                    {
                        has_code = true;
                        current_state = State1;
                    }
                    else if (line[i] == '\"')
                    {
                        has_code = true;
                        current_state = State2;
                    }
                    else if (!std::isspace(line[i]))
                    {
                        has_code = true;
                    }
                }
                break;
                case State1:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\'')
                        current_state = State0;
                }
                break;
                case State2:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\"')
                        current_state = State0;
                }
                break;
                case State3:
                {
                    has_comment = true;
                    if (line[i] == '*')
                    {
                        if (i + 1 < line.length())
                        {
                            if (line[i + 1] == '/')
                            {
                                current_state = State0;
                                in_multi_line_comment = false;
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
                break;
                }
            }

            if (has_code)
                file_stats.code_line++;
            else if (has_comment) // if it does not have code but comment:
                file_stats.comment_line++;

            file_stats.total_line++;
        }
        file.close();
        return file_stats;
    }

    types::FileStats file_analyzer_py(fs::path path)
    {
        types::FileStats file_stats;
        // std::cout << "Debugging...\n";
        DFA_Machine current_state = State0;

        bool in_multi_line_comment = 0;
        char mark = '0';
        std::string line;
        std::ifstream file(path, std::ios::in);

        if (!file.is_open())
        {
            std::cerr << "File '" << path << "' could not open for reading.\n";
        }

        while (std::getline(file, line))
        {
            bool is_blank = true;
            for (char c : line)
            {
                if (!std::isspace(c))
                {
                    is_blank = false;
                    break;
                }
            }
            if (is_blank)
            {
                file_stats.blank_line++;
                file_stats.total_line++;
                continue;
            }

            bool has_code = 0;
            bool has_comment = 0;
            if (in_multi_line_comment)
                current_state = State3;

            for (size_t i = 0; i < line.length(); i++)
            {
                switch (current_state)
                {
                case State0:
                {
                    if (line[i] == '#') // Yorum girişi
                    {
                        has_comment = true;
                        i = line.length();
                    }
                    else if (line[i] == '\'')
                    {
                        current_state = State1;
                    }
                    else if (line[i] == '\"')
                    {
                        current_state = State2;
                    }
                    else if (!std::isspace(line[i]))
                    {
                        has_code = true;
                    }
                }
                break;
                case State1:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\'')
                    {
                        if (i + 1 < line.length() && line[i + 1] == '\'')
                        {
                            i++;
                            in_multi_line_comment = true;
                            current_state = State3;
                            has_code = true;
                            mark = '\'';
                        }
                        else
                            current_state = State0;
                    }
                }
                break;
                case State2:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\"') // The second mark, end of string or
                    {
                        if (i + 1 < line.length() && line[i + 1] == '\"') // beginning of the multiline comment
                        {
                            i++;
                            in_multi_line_comment = true;
                            current_state = State3;
                            has_code = true;
                            mark = '\"';
                        }
                        else
                            current_state = State0;
                    }
                }
                break;
                case State3:
                {
                    has_comment = true;

                    if (line[i] == mark && i + 2 < line.length() && line[i + 1] == mark && line[i + 2] == mark)
                    {
                        in_multi_line_comment = false;
                        current_state = State0;
                        i += 2;
                    }
                }
                break;
                }
            }

            if (has_code)
                file_stats.code_line++;
            else if (has_comment) // if it does not have code but comment:
                file_stats.comment_line++;

            file_stats.total_line++;
        }
        file.close();
        return file_stats;
    }
}