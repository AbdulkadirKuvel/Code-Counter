#pragma once

#include <map>
#include <string>
#include <ostream>
#include <types.hpp>

class IReportFormatter
{
public:
    virtual ~IReportFormatter() = default;

    virtual void format(std::ostream &os, const std::map<std::string, types::FileStats> &stats) const = 0;
};
