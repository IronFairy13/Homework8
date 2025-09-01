#include "bayan/hasher.hpp"
#include "bayan/io_stats.hpp"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <fstream>
#include <cstdio>

namespace bayan
{
    namespace md5m = boost::uuids::detail;

    FileHasher::FileHasher(fs::path p, std::uintmax_t sz, std::size_t bs, Algo a)
        : path_(std::move(p)), size_(sz), block_size_(bs), algo_(a)
    {
        blocks_ = static_cast<std::size_t>((size_ + block_size_ - 1) / block_size_);
        cache_.resize(blocks_);
    }

    HashStr FileHasher::hash_at(std::size_t idx) const
    {
        if (idx >= blocks_)
            throw std::out_of_range("block index");
        if (cache_[idx].has_value())
        {
            ++g_stats.cache_hits;
            return *cache_[idx];
        }

        std::vector<unsigned char> buf(block_size_, 0u);
        std::ifstream ifs(path_.string(), std::ios::binary);
        if (!ifs)
            throw std::runtime_error("cannot open: " + path_.string());

        const std::uintmax_t offset = static_cast<std::uintmax_t>(idx) * block_size_;
        ifs.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
        if (ifs.good())
        {
            ifs.read(reinterpret_cast<char *>(buf.data()), static_cast<std::streamsize>(block_size_));
        }

        HashStr out;
        if (algo_ == Algo::CRC32)
        {
            boost::crc_32_type crc;
            crc.process_bytes(buf.data(), block_size_);
            unsigned int v = crc.checksum();
            char hex[9];
            std::snprintf(hex, sizeof(hex), "%o8x", v);
            out.s.assign(hex);
        } else {
            md5d::md5 md;
            md.process_bytes(buf.data(), block_size_);
            md5d::md5::digest_type d;
            md.get_digest_type(d);
            char hex[33];
            std::snprintf(hex, sizeof(hex), "%08x%08x%08x%08x", d[0], d[1], d[2], d[3]);
            out.s.assign(hex);
        }

        cache_[idx] = out;
        ++g_stats.unique_reads;
        return out;
    }
}