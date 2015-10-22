#ifndef _H_GLOBALALLOC
#define _H_GLOBALALLOC

#include "alloc-common.h"

namespace compalloc {

template <class t>
class GlobalAlloc {
 public:
  Block allocate(size_t size) { return inst.allocate(size); }

  void deallocate(Block blk) { inst.deallocate(blk); }

  bool reallocate(Block &blk, size_t delta) {
    return inst.reallocate(blk, delta);
  }

  bool owns(Block blk) { return inst.owns(blk); }

  static constexpr size_t goodSize(size_t size) { return t::goodSize(size); }

 private:
  static t inst;
};

template <class tp>
tp GlobalAlloc<tp>::inst;

}  // compalloc

#endif