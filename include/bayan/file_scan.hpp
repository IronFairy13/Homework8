#pragma once 
#include <boost/filesystem.hpp>
#include <vector>
#include <memory>
#include "config.hpp"
#include "hasher.hpp"

namespace bayan {

    namespace fs = boost::filesystem;

    struct FileEntry {
        fs::path path;
        std::uintmax_t size{0};
        std::shared_ptr<FileHasher> hasher;
    };

    std::vector<FileEntry> collect_files(const Config& cfg);
}