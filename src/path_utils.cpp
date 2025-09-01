#include "bayan/path_utils.hpp"
#include <algorithm>
#include <cwctype>

namespace bayan {

bool path_starts_with(const fs::path& p, const fs::path& root) {
    auto pit = p.begin(), pend = p.end();
    auto rit = root.begin(), rend = root.end();
    for (; rit != rend; ++rit, ++pit) {
        if (pit == pend) return false;
#ifdef _WIN32
        auto a = pit->wstring(); auto b = rit->wstring();
        std::transform(a.begin(), a.end(), a.begin(), ::towlower);
        std::transform(b.begin(), b.end(), b.begin(), ::towlower);
        if (a != b) return false;
#else
        if (*pit != *rit) return false;
#endif
    }
    return true;
}

bool is_excluded(const fs::path& p, const std::vector<fs::path>& excludes) {
    for (auto& ex : excludes) if (path_starts_with(p, ex)) return true;
    return false;
}

} 

