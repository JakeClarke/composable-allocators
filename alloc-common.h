#ifndef _H_ALLOC_COMMON
#define _H_ALLOC_COMMON

#include <cstddef>
#include <cstring>

struct Block {
  void *ptr;
  size_t size;
};

struct Header
{
	size_t size;
};

namespace utils {
void copy(Block dst, Block src) { memcpy(dst.ptr, src.ptr, src.size); }

template <class dst_t, class src_t> 
bool move(dst_t *dst, src_t *src, Block &blk, size_t delta) {
  auto newBlk = dst->allocate(blk.size + delta);
  if (newBlk.ptr == nullptr) {
    return false;
  }
  copy(newBlk, blk);
  src->deallocate(blk);
  blk = newBlk;
  return true;
}

constexpr size_t roundToAlignment(size_t size, size_t allign) {
	return size + (allign - (size % allign));
}
}

#endif