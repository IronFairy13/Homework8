#pragma once
#include <vector>
#include "file_scan.hpp"
#include "config.hpp"

namespace bayan {

    void find_and_output(std::vector<FileEntry>& files, const Config& cfg);
}