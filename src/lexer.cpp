#include <lexer.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <print>

namespace lexer
{
    /// @brief Analyzes the given string block for blank lines
    /// @param content the block of string
    /// @return LineMetrics
    inline LineMetrics analyze_lines(std::string_view content)
    {
        LineMetrics metrics;
        if (content.empty())
            return metrics;

        size_t pos = 0;
        const size_t len = content.length();

        while (pos < len)
        {
            size_t next_nl = content.find('\n', pos);
            bool is_last_line = (next_nl == std::string_view::npos);
            size_t end_pos = is_last_line ? len : next_nl;

            metrics.total++;

            size_t first_char = content.find_first_not_of(" \t\r", pos);

            if (first_char == std::string_view::npos || first_char >= end_pos)
            {
                metrics.blank++;
            }

            if (is_last_line)
                break;
            pos = next_nl + 1;
        }
        return metrics;
    }

    /// @brief analyzes files that has comments like C
    /// @param content
    /// @param stats
    void file_analyzer_c(std::string_view content, types::FileStats &stats)
    {
        LineMetrics file_metrics = analyze_lines(content);
        stats.total_line = file_metrics.total;
        stats.blank_line = file_metrics.blank;

        size_t pos = 0;
        size_t comment_start_pos = 0;
        const size_t len = content.length();
        DFA_Machine current_state = DFA_Machine::IN_CODE;

        while (pos < len)
        {
            switch (current_state)
            {
            case IN_CODE:
            {
                size_t next_pos = content.find_first_of("/'\"", pos);

                if (next_pos == std::string_view::npos)
                {
                    pos = len;
                    break;
                }

                pos = next_pos;

                char trigger = content[pos];

                if (trigger == '/')
                {
                    if (pos + 1 < len)
                    {
                        // is this a single line comment?
                        char next_chr = content[pos + 1];
                        if (next_chr == '/')
                        {
                            // this is a single line comment.
                            // Look-behind
                            bool has_code_before = false;
                            if (pos > 0)
                            {
                                size_t line_start = content.find_last_of('\n', pos - 1);
                                line_start = (line_start == std::string_view::npos) ? 0 : line_start + 1;
                                size_t non_ws = content.find_first_not_of(" \t\r", line_start);

                                if (non_ws != std::string_view::npos && non_ws < pos)
                                {
                                    has_code_before = true;
                                }
                            }

                            if (!has_code_before)
                            {
                                stats.comment_line++;
                            }
                            // else
                            // {
                            // This part will be counted in the final calculation.
                            // }

                            size_t eol = content.find('\n', pos);
                            pos = (eol == std::string_view::npos) ? len : eol;
                        }
                        else if (next_chr == '*')
                        {
                            // This is a multiline comment.
                            current_state = DFA_Machine::IN_MULTI_COMMENT;
                            comment_start_pos = pos;
                            pos += 2;
                        }
                        else
                        {
                            pos++;
                        }
                    }
                }
                else if (trigger == '"')
                {
                    current_state = DFA_Machine::IN_DOUBLE_STRING;
                    pos++;
                }
                else if (trigger == '\'')
                {
                    current_state = DFA_Machine::IN_SINGLE_STRING;
                    pos++;
                }
                break;
            }
            case IN_SINGLE_STRING:
            {
                size_t end_str = content.find('\'', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            case IN_DOUBLE_STRING:
            {
                size_t end_str = content.find('\"', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            case IN_MULTI_COMMENT:
            {
                size_t end_comment = content.find("*/", pos);

                std::string_view comment_block;

                if (end_comment == std::string_view::npos)
                {
                    comment_block = content.substr(comment_start_pos);
                    pos = len;
                }
                else
                {
                    comment_block = content.substr(comment_start_pos, (end_comment + 2) - comment_start_pos);
                    current_state = DFA_Machine::IN_CODE;
                    pos = end_comment + 2;
                }

                // Calculate the length of the comment
                LineMetrics comment_metrics = analyze_lines(comment_block);
                size_t real_comment_lines = comment_metrics.total - comment_metrics.blank;

                bool has_code_before = false;
                if (comment_start_pos > 0)
                {
                    size_t line_start = content.find_last_of('\n', comment_start_pos - 1);
                    line_start = (line_start == std::string_view::npos) ? 0 : line_start + 1;

                    size_t non_ws = content.find_first_not_of(" \t\r", line_start);
                    has_code_before = (non_ws != std::string_view::npos && non_ws < comment_start_pos);
                }

                bool has_code_after = false;
                bool is_single_line = (comment_metrics.total == 1);

                if (end_comment != std::string_view::npos)
                {
                    size_t next_pos = end_comment + 2;
                    size_t line_end = content.find('\n', next_pos);
                    line_end = (line_end == std::string_view::npos) ? len : line_end;

                    size_t non_ws = content.find_first_not_of(" \t\r", next_pos);
                    has_code_after = (non_ws != std::string_view::npos && non_ws < line_end);
                }

                // if the line has code before comment "bool a; /* bool"
                if (has_code_before && real_comment_lines > 0)
                {
                    real_comment_lines--;
                }

                // if the line has code after comment : "*/ bool a;"
                if (!is_single_line && has_code_after && real_comment_lines > 0)
                {
                    real_comment_lines--;
                }
                stats.comment_line += real_comment_lines;
                break;
            }
            default:
                break;
            }
        }
        stats.code_line = stats.total_line - (stats.comment_line + stats.blank_line);
    }

    void file_analyzer_py(std::string_view content, types::FileStats &stats)
    {
        LineMetrics file_metrics = analyze_lines(content);
        stats.total_line = file_metrics.total;
        stats.blank_line = file_metrics.blank;

        size_t pos = 0;
        // size_t comment_start_pos = 0;
        const size_t len = content.length();
        DFA_Machine current_state = DFA_Machine::IN_CODE;

        while (pos < len)
        {
            switch (current_state)
            {
            case IN_CODE:
            {
                size_t next_pos = content.find_first_of("#'\"", pos);

                if (next_pos == std::string_view::npos)
                {
                    pos = len;
                    break;
                }

                pos = next_pos;

                char trigger = content[pos];

                if (trigger == '#')
                {
                    // This is a single line comment.
                    // Look-behind

                    bool has_code_before = false;
                    if (pos > 0)
                    {
                        size_t line_start = content.find_last_of('\n', pos - 1);
                        line_start = (line_start == std::string_view::npos) ? 0 : line_start + 1;
                        size_t non_ws = content.find_first_not_of(" \t\r", line_start);

                        if (non_ws != std::string_view::npos && non_ws < pos)
                        {
                            has_code_before = true;
                        }
                    }

                    if (!has_code_before)
                    {
                        stats.comment_line++;
                    }
                    // else
                    // {
                    // This part will be counted in the final calculation.
                    // }

                    // Jump the position to next line.
                    size_t eol = content.find('\n', pos);
                    pos = (eol == std::string_view::npos) ? len : eol;
                }
                else if (trigger == '"')
                {
                    // FEATURE: Create heuristic structure for better accuracy.
                    current_state = DFA_Machine::IN_DOUBLE_STRING;
                    pos++;
                }
                else if (trigger == '\'')
                {
                    current_state = DFA_Machine::IN_SINGLE_STRING;
                    pos++;
                }
                break;
            }
            case IN_SINGLE_STRING:
            {
                size_t end_str = content.find('\'', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            case IN_DOUBLE_STRING:
            {
                size_t end_str = content.find('\"', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            // No multiline comment processing yet.
            // case IN_MULTI_COMMENT:
            // {
            //     break;
            // }
            default:
                break;
            }
        }
        stats.code_line = stats.total_line - (stats.comment_line + stats.blank_line);
    }

    void file_analyzer_xml(std::string_view content, types::FileStats &stats)
    {
        LineMetrics file_metrics = analyze_lines(content);
        stats.total_line = file_metrics.total;
        stats.blank_line = file_metrics.blank;

        size_t pos = 0;
        size_t comment_start_pos = 0;
        const size_t len = content.length();
        DFA_Machine current_state = DFA_Machine::IN_CODE;

        while (pos < len)
        {
            switch (current_state)
            {
            case IN_CODE:
            {
                size_t next_pos = content.find_first_of("<\"'", pos);

                if (next_pos == std::string_view::npos)
                {
                    pos = len;
                    break;
                }

                pos = next_pos;

                char trigger = content[pos];

                if (trigger == '<')
                {
                    if (pos + 3 < len && content.substr(pos, 4) == "<!--")
                    {
                        current_state = DFA_Machine::IN_MULTI_COMMENT;
                        comment_start_pos = pos;
                        pos += 4;
                    }
                    else
                    {
                        pos++;
                    }
                }
                else if (trigger == '"')
                {
                    current_state = DFA_Machine::IN_DOUBLE_STRING;
                    pos++;
                }
                else if (trigger == '\'')
                {
                    current_state = DFA_Machine::IN_SINGLE_STRING;
                    pos++;
                }
                break;
            }
            case IN_SINGLE_STRING:
            {
                size_t end_str = content.find('\'', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            case IN_DOUBLE_STRING:
            {
                size_t end_str = content.find('\"', pos);
                if (end_str == std::string_view::npos)
                {
                    pos = len;
                }
                else
                {
                    pos = end_str + 1;
                    current_state = IN_CODE;
                }
                break;
            }
            case IN_MULTI_COMMENT:
            {
                size_t end_comment = content.find("-->", pos);

                std::string_view comment_block;

                if (end_comment == std::string_view::npos)
                {
                    comment_block = content.substr(comment_start_pos);
                    pos = len;
                }
                else
                {
                    comment_block = content.substr(comment_start_pos, (end_comment + 3) - comment_start_pos);
                    current_state = DFA_Machine::IN_CODE;
                    pos = end_comment + 3;
                }

                LineMetrics comment_metrics = analyze_lines(comment_block);
                size_t real_comment_lines = comment_metrics.total - comment_metrics.blank;

                bool has_code_before = false;
                if (comment_start_pos > 0)
                {
                    size_t line_start = content.find_last_of('\n', comment_start_pos - 1);
                    line_start = (line_start == std::string_view::npos) ? 0 : line_start + 1;

                    size_t non_ws = content.find_first_not_of(" \t\r", line_start);
                    has_code_before = (non_ws != std::string_view::npos && non_ws < comment_start_pos);
                }

                bool has_code_after = false;
                bool is_single_line = (comment_metrics.total == 1);

                if (end_comment != std::string_view::npos)
                {
                    size_t next_pos = end_comment + 3;
                    size_t line_end = content.find('\n', next_pos);
                    line_end = (line_end == std::string_view::npos) ? len : line_end;

                    size_t non_ws = content.find_first_not_of(" \t\r", next_pos);
                    has_code_after = (non_ws != std::string_view::npos && non_ws < line_end);
                }

                // if the line has code before comment : "<p> <!--"
                if (has_code_before && real_comment_lines > 0)
                {
                    real_comment_lines--;
                }

                // if the line has code after comment : "--> <p>"
                if (!is_single_line && has_code_after && real_comment_lines > 0)
                {
                    real_comment_lines--;
                }

                stats.comment_line += real_comment_lines;
                break;
            }
            default:
                break;
            }
        }
        stats.code_line = stats.total_line - (stats.comment_line + stats.blank_line);
    }
}