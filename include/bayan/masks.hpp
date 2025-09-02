#pragma once
#include <boost/regex.hpp>
#include <string>
#include <vector>

namespace bayan
{
    std::string glob_to_regex(const std::string &glob);
    bool name_matches_any(const std::string &name, const std::vector<boost::regex> &masks);

}