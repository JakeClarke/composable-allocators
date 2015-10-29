#ifndef _H_INCLUDE_COMPALLOC_FREE_LIST_ALLOC
#define _H_INCLUDE_COMPALLOC_FREE_LIST_ALLOC

#include "alloc-common.h"

namespace compalloc {

template <class alloc>
class FreeListAlloc : private alloc {
 public:
  FreeListAlloc():size(0), root({nullptr, 0}), tail(&root) {

  }

  Block allocate(size_t size) {
    if (size == 0) {
      return alloc::allocate(size);
    }

    Block *curr = &root;
    Block *prev = nullptr;
    while (curr->ptr != nullptr && curr->size != goodSize(size)) {
      prev = curr;
      curr = next(*curr);
    }
    if (curr->ptr != nullptr) {
      if (curr == tail) {
        tail = prev;
      } else {
        prev = next(*curr);
      }

      --size;
      return *curr;
    } else {
      return alloc::allocate(size);
    }
  }

  void deallocate(Block blk) {
    // assert(usedSlots < numSlots);
    *tail = blk;
    tail = next(*tail);
    *tail = {nullptr, 0};
    ++size;
  }

  bool reallocate(Block &blk, size_t delta) {
    return alloc::reallocate(blk, delta);
  }

  bool owns(Block blk) { return alloc::owns(blk); }

  constexpr static size_t goodSize(size_t size) {
    return alloc::goodSize(size);
  }

  ~FreeListAlloc() {
    Block curr = root, n;
    while (curr.ptr != nullptr) {
      n = *next(curr);
      alloc::deallocate(curr);
      curr = n;
    }
  }

 private:
  Block *next(Block blk) { return static_cast<Block *>(blk.ptr); }

  Block root;
  Block *tail;
  size_t size;
};
}

#endif