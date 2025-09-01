#include "bayan/lex_compare.hpp"

namespace bayan {

bool LexComparator::operator()(std::size_t i, std::size_t j) const {
    const auto& A = (*files)[i];
    const auto& B = (*files)[j];
    for (std::size_t k = 0; k < nblocks; ++k) {
        const auto ha = A.hasher->hash_at(k);
        const auto hb = B.hasher->hash_at(k);
        if (ha != hb) return ha < hb;
    }
    return A.path.string() < B.path.string();
}

bool equals_full(const FileEntry& A, const FileEntry& B) {
    const std::size_t nblocks = A.hasher->num_blocks();
    for (std::size_t k = 0; k < nblocks; ++k)
        if (A.hasher->hash_at(k) != B.hasher->hash_at(k)) return false;
    return true;
}

} 
