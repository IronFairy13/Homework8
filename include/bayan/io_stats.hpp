#pragma once
#include <cstddef>

namespace bayan
{

    struct IoStats
    {
        std::size_t unique_reads = 0;
        std::size_t cache_hits = 0;
    };

    extern IoStats g_stats;
}