#pragma once 
#include <boost/filesystem.hpp>
#include <vector>

namespace bayan {
    namespace fs = boost::filesystem;

    bool path_starts_with(const fs::path& p, const fs::path& root);
    bool is_excluded( const fs::path& p, const std::vector<fs::path>& excludes);
}