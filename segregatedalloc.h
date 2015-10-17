#ifndef _H_SEGREGATED_ALLOC
#define _H_SEGREGATED_ALLOC

template <typename small, typename big, size_t thresshold>
class SegregatedAlloc : private small, private big {
 public:
  Block allocate(size_t size) {
    if (size < thresshold) {
      return small::allocate(size);
    }
    return big::allocate(size);
  }

  void deallocate(Block blk) {
    if (blk.size < thresshold) {
      small::deallocate(blk);
    } else {
      big::deallocate(blk);
    }
  }

  void owns(Block blk) {
    if (blk.size < thresshold) {
      return small::owns(blk);
    }
    return big::owns(blk);
  }

  bool reallocate(Block &blk, size_t delta) {
    if (blk.size < thresshold) {
      if (blk.size + delta < thresshold) {
        return small::reallocate(blk, delta);
      }

      return utils::move<big, small>(blk, delta);

    } else {
      return big::reallocate(blk, delta);
    }
  }

  constexpr static size_t goodSize(size_t size) {
    size_t res = size;
    if (size < thresshold) {
      res = small::goodSize(size);
    } else {
      res = big::goodSize(size);
    }
    return res;
  }
};

#endif