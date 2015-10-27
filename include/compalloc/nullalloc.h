#ifndef _H_INCLUDE_COMPALLOC_NULLALLOC
#define _H_INCLUDE_COMPALLOC_NULLALLOC

#include "alloc-common.h"
#include <cassert>

namespace compalloc {

class NullAlloc {
 public:
  Block allocate(size_t) {
    return {nullptr, 0};
  }

  void deallocate(Block) { assert(false); }

  bool owns(Block) { return false; }

  bool reallocate(Block &, size_t) { return false; }

  constexpr static size_t goodSize(size_t size) { return size; }
};
}

#endif