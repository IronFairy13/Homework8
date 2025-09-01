#pragma once
#include <vector>
#include <cstddef>
#include "file_scan.hpp"

namespace bayan
{

    struct LexComparator
    {

        const std::vector<FileEntry> *files{};
        std::size_t nblocks{0};
        bool operator()(std::size_t y, std::size_t j) const;
    };
}