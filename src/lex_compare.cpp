#include "bayan/lex_compare.hpp"

namespace bayan {

bool LexComparator::operator()(std::size_t i, std::size_t j) const {
  const auto &A = (*files)[i];
  const auto &B = (*files)[j];
  for (std::size_t k = 0; k < nblocks; ++k) {
    const auto ha = A.hasher->hash_at(k);
    const auto hb = B.hasher->hash_at(k);
    if (ha != hb)
      return ha < hb;
  }
  return A.path.string() < B.path.string();
}

bool equals_full(const FileEntry &a, const FileEntry &b) {
  if (a.size != b.size)
    return false;

  auto hasher_a =
      std::make_shared<FileHasher>(a.path, a.size, 4096, Algo::CRC32);
  auto hasher_b =
      std::make_shared<FileHasher>(b.path, b.size, 4096, Algo::CRC32);

  if (hasher_a->num_blocks() != hasher_b->num_blocks())
    return false;

  for (std::size_t i = 0; i < hasher_a->num_blocks(); ++i) {
    if (hasher_a->hash_at(i) != hasher_b->hash_at(i)) {
      return false;
    }
  }
  return true;
}

bool equals_lex(const FileEntry &a, const FileEntry &b) {
  return a.path < b.path;
}

} // namespace bayan
