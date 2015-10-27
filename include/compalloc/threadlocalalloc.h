#ifndef _H_INCLUDE_COMPALLOC_THREAD_LOCAL_ALLOC
#define _H_INCLUDE_COMPALLOC_THREAD_LOCAL_ALLOC

#include "alloc-common.h"

namespace compalloc {

template <typename threadlocalalloc>
class ThreadLocalAlloc {
 public:
  Block allocate(size_t size) { return tll.allocate(size); }

  void deallocate(Block blk) { tll.deallocate(blk); }

  bool owns(Block blk) { return tll.owns(blk); }

  bool reallocate(Block &blk, size_t delta) {
    return tll.reallocate(blk, delta);
  }

  static constexpr size_t goodSize(size_t size) {
    return threadlocalalloc::goodSize(size);
  }

 private:
  static thread_local threadlocalalloc tll;
};

template <typename t>
thread_local t ThreadLocalAlloc<t>::tll;
} // compalloc

#endif