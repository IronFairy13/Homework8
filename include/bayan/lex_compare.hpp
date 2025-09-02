#pragma once
#include "file_scan.hpp"
#include <cstddef>
#include <vector>

namespace bayan {

struct LexComparator {

  const std::vector<FileEntry> *files{};
  std::size_t nblocks{0};
  bool operator()(std::size_t y, std::size_t j) const;
};
bool equals_full(const FileEntry &a, const FileEntry &b);
bool equals_lex(const FileEntry &a, const FileEntry &b);
} // namespace bayan