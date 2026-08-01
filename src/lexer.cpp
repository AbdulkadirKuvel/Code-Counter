#include <lexer.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <print>
namespace lexer
{

    /**
     * @brief for analyzing files that has comments like C
     * @param path path of the file
     * @returns types::FileStats
     */
    types::FileStats file_analyzer_c(const fs::path &path)
    {
        types::FileStats file_stats;
        DFA_Machine current_state = IN_CODE;

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
                current_state = IN_MULTI_COMMENT;

            for (size_t i = 0; i < line.length(); i++)
            {
                switch (current_state)
                {
                case IN_CODE:
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
                                current_state = IN_MULTI_COMMENT;
                            }
                        }
                    }
                    else if (line[i] == '\'')
                    {
                        has_code = true;
                        current_state = IN_SINGLE_STRING;
                    }
                    else if (line[i] == '\"')
                    {
                        has_code = true;
                        current_state = IN_DOUBLE_STRING;
                    }
                    else if (!std::isspace(line[i]))
                    {
                        has_code = true;
                    }
                }
                break;
                case IN_SINGLE_STRING:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\'')
                        current_state = IN_CODE;
                }
                break;
                case IN_DOUBLE_STRING:
                {
                    if (line[i] == '\\')
                    {
                        i++;
                        continue;
                    }
                    else if (line[i] == '\"')
                        current_state = IN_CODE;
                }
                break;
                case IN_MULTI_COMMENT:
                {
                    has_comment = true;
                    if (line[i] == '*')
                    {
                        if (i + 1 < line.length())
                        {
                            if (line[i + 1] == '/')
                            {
                                current_state = IN_CODE;
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

    types::FileStats file_analyzer_py(const fs::path &path)
    {
        types::FileStats file_stats;
        DFA_Machine current_state = IN_CODE;

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
                current_state = IN_MULTI_COMMENT;

            for (size_t i = 0; i < line.length(); i++)
            {
                switch (current_state)
                {
                case IN_CODE:
                {
                    if (line[i] == '#') // Yorum girişi
                    {
                        has_comment = true;
                        i = line.length();
                    }
                    else if (line[i] == '\'')
                    {
                        current_state = IN_SINGLE_STRING;
                    }
                    else if (line[i] == '\"')
                    {
                        current_state = IN_DOUBLE_STRING;
                    }
                    else if (!std::isspace(line[i]))
                    {
                        has_code = true;
                    }
                }
                break;
                case IN_SINGLE_STRING:
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
                            current_state = IN_MULTI_COMMENT;
                            has_code = true;
                            mark = '\'';
                        }
                        else
                            current_state = IN_CODE;
                    }
                }
                break;
                case IN_DOUBLE_STRING:
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
                            current_state = IN_MULTI_COMMENT;
                            has_code = true;
                            mark = '\"';
                        }
                        else
                            current_state = IN_CODE;
                    }
                }
                break;
                case IN_MULTI_COMMENT:
                {
                    has_comment = true;

                    if (line[i] == mark && i + 2 < line.length() && line[i + 1] == mark && line[i + 2] == mark)
                    {
                        in_multi_line_comment = false;
                        current_state = IN_CODE;
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