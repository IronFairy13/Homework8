#include "bayan/file_scan.hpp"
#include "bayan/masks.hpp"
#include "bayan/path_utils.hpp"

namespace bayan {

static void collect_in_dir(const fs::path& dir, const Config& cfg, int level,
                           const std::vector<boost::regex>& masks,
                           std::vector<FileEntry>& out)
{
    if (!fs::exists(dir) || !fs::is_directory(dir)) return;

    fs::directory_iterator it(dir), end;
    for (; it != end; ++it) {
        const fs::path p = it->path();
        if (is_excluded(p, cfg.exclude_dirs)) continue;

        boost::system::error_code ec;
        if (fs::is_directory(p, ec)) {
            if (level != 0) collect_in_dir(p, cfg, level < 0 ? -1 : level - 1, masks, out);
        } else if (fs::is_regular_file(p, ec)) {
            std::uintmax_t sz = fs::file_size(p, ec);
            if (ec) continue;
            if (sz <= cfg.min_size) continue;
            if (!name_matches_any(p.filename().string(), masks)) continue;

            out.push_back(FileEntry{ fs::absolute(p), sz, nullptr });
        }
    }
}

std::vector<FileEntry> collect_files(const Config& cfg) {
    std::vector<FileEntry> files;
    for (auto& d : cfg.include_dirs) collect_in_dir(d, cfg, cfg.level, cfg.masks, files);
    return files;
}

} 
