#pragma once

#include <string>

namespace bayan
{

    struct HashStr
    {
        std::string s;
        bool operator==(const HashStr &o) const noexcept { return s == o.s; }
        bool operator!=(const HashStr &o) const noexcept { return s != o.s; }
        bool operator<(const HashStr &o) const noexcept { return s < o.s; }
    };
}