#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <optional>
#include <cstdint>
#include <memory>
#include "config.hpp"
#include "hashing.hpp"

namespace bayan
{

    namespace fs = boost::filesystem;

    class FileHasher
    {
    public:
        FileHasher() = default;
        FileHasher(fs::path p, std::uintmax_t sz, std::size_t bs, Algo a);

        std::size_t num_blocks() const { return blocks_; }
        HashStr hash_at(std::size_t idx) const;

        const fs::path &path() const { return path_; }

        private:
        fs::path path_;
        std::uintmax_t size_ = 0;
        std::size_t block_size_ = 4096;
        Algo algo_ = Algo::CRC32;
        std::size_t blocks_ = 0;
        mutable std::vector<std::optional<HashStr>> cache_;

    };

}