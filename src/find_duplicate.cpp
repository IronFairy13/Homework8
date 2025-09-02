#include "bayan/find_duplicates.hpp"
#include "bayan/lex_compare.hpp"  // Добавляем этот include
#include "bayan/hasher.hpp"
#include "bayan/config.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace bayan {

namespace fs = boost::filesystem;

void find_and_output(std::vector<FileEntry>& files, const Config& cfg) {

    std::unordered_map<std::uintmax_t, std::vector<std::size_t>> by_size;
    by_size.reserve(files.size());
    for (std::size_t i = 0; i < files.size(); ++i) by_size[files[i].size].push_back(i);

    const auto mk_hasher = [&](const fs::path& p, std::uintmax_t sz) {
        return std::make_shared<FileHasher>(p, sz, cfg.block_size, cfg.algo);
    };

   

    for (auto& [sz, idxs] : by_size) {
        if (idxs.size() < 2) continue;

        for (auto i : idxs) if (!files[i].hasher) files[i].hasher = mk_hasher(files[i].path, files[i].size);
        const std::size_t nblocks = files[idxs[0]].hasher->num_blocks();

        LexComparator cmp{ &files, nblocks };
        std::sort(idxs.begin(), idxs.end(), cmp);

        std::vector<std::size_t> cluster;
        auto flush_cluster = [&](){
            if (cluster.size() >= 2) {
                if (!cfg.graphviz) {
                    for (auto i : cluster) std::cout << files[i].path.string() << "\n";
                    std::cout << "\n";
                } else {
                    std::cout << "subgraph cluster_" << files[cluster[0]].size << " {\n";
                    for (auto i : cluster)
                        std::cout << "  \"" << files[i].path.string() << "\";\n";
                    std::cout << "}\n";
                }
            }
            cluster.clear();
        };

        cluster.push_back(idxs[0]);
        for (std::size_t t = 1; t < idxs.size(); ++t) {
            auto prev = cluster.back();
            auto cur  = idxs[t];
            if (equals_full(files[prev], files[cur])) {
                cluster.push_back(cur);
            } else {
                flush_cluster();
                cluster.push_back(cur);
            }
        }
        flush_cluster();
    }
}

} 
