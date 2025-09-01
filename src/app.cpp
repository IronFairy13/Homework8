#include "bayan/app.hpp"
#include "bayan/config.hpp"
#include "bayan/masks.hpp"
#include "bayan/file_scan.hpp"
#include "bayan/find_duplicates.hpp"
#include "bayan/io_stats.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <algorithm>

namespace bayan {

namespace po = boost::program_options;

int run_app(int argc, char** argv) {
    Config cfg;
    std::vector<std::string> include_s, exclude_s, masks_s;
    std::string algo_s;

    po::options_description desc("bayan-lexi: duplicate finder via lexicographic lazy hashing");
    desc.add_options()
        ("help,h", "show help")
        ("include,I", po::value<std::vector<std::string>>(&include_s)->multitoken()->required(),
            "directories to scan (one or more)")
        ("exclude,X", po::value<std::vector<std::string>>(&exclude_s)->multitoken(),
            "directories to exclude")
        ("level,L", po::value<int>(&cfg.level)->default_value(-1),
            "depth: 0=only given dirs, -1=unlimited")
        ("min-size", po::value<std::uintmax_t>(&cfg.min_size)->default_value(1),
            "min file size (strictly >)")
        ("block-size,S", po::value<std::size_t>(&cfg.block_size)->default_value(4096),
            "block size S (bytes)")
        ("algo,a", po::value<std::string>(&algo_s)->default_value("crc32"),
            "hash algorithm: crc32|md5")
        ("mask,m", po::value<std::vector<std::string>>(&masks_s)->multitoken(),
            "filename masks (glob, case-insensitive)")
        ("trace-io", po::bool_switch(&cfg.trace_io)->default_value(false),
            "print I/O stats to stderr")
        ("graphviz", po::bool_switch(&cfg.graphviz)->default_value(false),
            "output clusters as Graphviz subgraphs (experimental)");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) { std::cout << desc << "\n"; return 0; }
        po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n" << desc << "\n";
        return 2;
    }

    for (auto& s : include_s) cfg.include_dirs.emplace_back(s);
    for (auto& s : exclude_s) cfg.exclude_dirs.emplace_back(s);

    std::string lower = algo_s;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    if (lower == "crc32") cfg.algo = Algo::CRC32;
    else if (lower == "md5") cfg.algo = Algo::MD5;
    else { std::cerr << "Unknown --algo: " << algo_s << "\n"; return 2; }

    for (auto& m : masks_s) cfg.masks.emplace_back(glob_to_regex(m), boost::regex::icase);

    auto files = collect_files(cfg);
    find_and_output(files, cfg);

    if (cfg.trace_io) {
        std::cerr << "[trace-io] unique disk block reads: " << g_stats.unique_reads
                  << ", cache hits: " << g_stats.cache_hits << "\n";
    }
    return 0;
}

} // namespace bayan
