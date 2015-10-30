#ifndef _H_INCLUDE_COMPALLOC_STATSALLOC
#define _H_INCLUDE_COMPALLOC_STATSALLOC

#include "alloc-common.h"

#include <atomic>

namespace compalloc {

struct AllocStats {
  size_t requested;
  size_t allocated;
  size_t deallocated;
  size_t current;
};

template <class alloc>
class StatsAlloc {
 public:
 	StatsAlloc(): requested(0), allocated(0), deallocated(0) {

 	}

  Block allocate(size_t size) {
    requested += size;
    auto blk = mainAlloc.allocate(size);
    allocated += blk.size;

    return blk;
  }

  void deallocate(Block blk) {
    deallocated += blk.size;
    mainAlloc.deallocate(blk);
  }

  bool reallocate(Block &blk, size_t delta) {
    return mainAlloc.reallocate(blk, delta);
  }

  AllocStats getStats() {
    const size_t a = allocated;
    const size_t d = deallocated;

    return {requested, a, d, a - d};
  }

 private:
  std::atomic_size_t requested;
  std::atomic_size_t allocated;
  std::atomic_size_t deallocated;

  alloc mainAlloc;
};
}

#endif