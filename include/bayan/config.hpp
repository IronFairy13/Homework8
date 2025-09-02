#pragma once
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <cstdint>

namespace bayan
{
    namespace fs = boost::filesystem;
    enum class Algo
    {
        CRC32,
        MD5
    };

    struct Config
    {
        std::vector<fs::path> include_dirs;
        std::vector<fs::path> exclude_dirs;
        int level = -1;
        std::uintmax_t min_size = 1;
        size_t block_size = 4096;
        Algo algo = Algo::CRC32;
        std::vector<boost::regex> masks;
        bool trace_io = false;
        bool graphviz = false;
    };
}