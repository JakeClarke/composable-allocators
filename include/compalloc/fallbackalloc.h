#ifndef _H_FALLBACK_ALLOC
#define _H_FALLBACK_ALLOC

#include "alloc-common.h"

namespace compalloc {

template <typename primary, typename secondary>
class FallBackAlloc : private primary, private secondary {
 public:
 public:
  Block allocate(size_t size) {
    auto blk = primary::allocate(size);
    if (blk.ptr != nullptr) {
      return blk;
    } else {
      return secondary::allocate(size);
    }
  }

  void deallocate(Block blk) {
    if (primary::owns(blk)) {
      primary::deallocate(blk);
    } else {
      secondary::deallocate(blk);
    }
  }

  bool reallocate(Block &blk, size_t delta) {
    if (primary::owns(blk)) {
      if (primary::reallocate(blk, delta)) {
        return true;
      }

      return utils::move<secondary, primary>(this, this, blk, delta);

    } else {
      if (utils::move<primary, secondary>(this, this, blk, delta)) {
        return true;
      }

      return secondary::reallocate(blk, delta);
    }
  }

  bool owns(Block blk) { return primary::owns(blk) || secondary::owns(blk); }

  constexpr static size_t goodSize(size_t size) {
    return primary::goodSize(size);
  }
};
}

#endif