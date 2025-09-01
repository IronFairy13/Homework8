#include "bayan/masks.hpp"

namespace bayan {

std::string glob_to_regex(const std::string& glob) {
    std::string rx = "^";
    rx.reserve(glob.size() * 2);
    for (char c : glob) {
        switch (c) {
            case '*': rx += ".*"; break;
            case '?': rx += ".";  break;
            case '.': rx += "\\."; break;
            case '\\': rx += "\\\\"; break;
            case '+': case '(': case ')': case '{': case '}':
            case '^': case '$': case '|': case '[': case ']':
                rx += '\\'; rx += c; break;
            default: rx += c; break;
        }
    }
    rx += "$";
    return rx;
}

bool name_matches_any(const std::string& name, const std::vector<boost::regex>& masks) {
    if (masks.empty()) return true;
    for (auto& r : masks)
        if (boost::regex_match(name, r)) return true;
    return false;
}

} 

