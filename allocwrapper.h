#ifndef _H_ALLOCWRAPPER
#define _H_ALLOCWRAPPER

#include "alloc-common.h"

struct MemHeader {
  size_t size;
};

struct MemTailer {
};

template <class alloc>
class AllocWrapper : private alloc {
 public:
  void* allocate(size_t size) {
    const auto neededSize = goodSize(size);
    auto blk = alloc::allocate(neededSize);
    if (blk.ptr == nullptr) {
    	return nullptr;
    }

    MemHeader* hdr = (MemHeader*)blk.ptr;
    hdr->size = blk.size;

    // return the start of the allocation.
    return (char *)blk.ptr + utils::roundToAlignment(sizeof(MemHeader), alignment());
  }

  void deallocate(void* ptr) {
    MemHeader* hdr = (MemHeader*)(((char *)ptr) - utils::roundToAlignment(sizeof(MemHeader), alignment()));
    Block blk = {hdr, hdr->size};
    alloc::deallocate(blk);
  }

  static constexpr size_t goodSize(size_t size) {
    return utils::roundToAlignment(sizeof(MemHeader), alignment()) +
           utils::roundToAlignment(size + sizeof(MemTailer), alignment());
  }

  static constexpr size_t alignment() { return 16; }
};

#endif
