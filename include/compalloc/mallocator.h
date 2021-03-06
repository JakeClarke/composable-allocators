#ifndef _H_INCLUDE_COMPALLOC_MALLOCATOR
#define _H_INCLUDE_COMPALLOC_MALLOCATOR

#include "alloc-common.h"
#include <cstdlib>

namespace compalloc {

class Mallocator {
 public:
  Block allocate(size_t size) {
    auto ptr = malloc(size);
    if (ptr == nullptr) {
      return {nullptr, 0};
    }

    return {ptr, size};
  }

  void deallocate(Block blk) { free(blk.ptr); }

  bool owns(Block) { return true; }

  bool reallocate(Block &blk, size_t delta) {
    const auto newSize = blk.size + delta;
    const auto ptr = realloc(blk.ptr, newSize);
    if (ptr != nullptr) {
      blk = {ptr, newSize};
      return true;
    }
    return false;
  }

  static constexpr size_t goodSize(size_t size) { return size; }

  static constexpr size_t alignment = 1;
};
}

#endif