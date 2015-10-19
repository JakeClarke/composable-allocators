#ifndef _H_ALLOCWRAPPER
#define _H_ALLOCWRAPPER

#include "alloc-common.h"

#include <cstring>

struct MemHeader {
  size_t size;
};

#define TAILER_TAG_SIZE 64

struct MemTailer {
  char tag[TAILER_TAG_SIZE];
};

template <class alloc>
class AllocWrapper : private alloc {
 public:
  void* allocate(size_t size, const char* tag) {
    const auto neededSize = goodSize(size);
    auto blk = alloc::allocate(neededSize);
    if (blk.ptr == nullptr) {
      return nullptr;
    }

    MemHeader* hdr = getHeader(blk);
    hdr->size = blk.size;

    MemTailer* tlr = getTailer(blk);
    memset(tlr, 0, sizeof(*tlr));
    if (tag != nullptr) {
      std::strncpy(tlr->tag, tag, TAILER_TAG_SIZE - 1);
    }

    // return the start of the allocation.
    return getDataBegin(blk);
  }

  void* allocate(size_t size) { return allocate(size, nullptr); }

  void deallocate(void* ptr) { alloc::deallocate(getBlock(ptr)); }

  static MemTailer* getTailer(void* ptr) {
    const size_t tailOffset =
        getHeader(ptr)->size - (sizeof(MemTailer) + utils::roundToAlignment(sizeof(MemHeader), alignment()));
    auto tlrPtr = (char*)ptr;
    tlrPtr = tlrPtr + tailOffset;
    return (MemTailer*)tlrPtr;
  }

  static MemTailer* getTailer(Block blk) {
    auto tlrPtr = (char*)blk.ptr;
    tlrPtr = tlrPtr + (blk.size - sizeof(MemTailer));
    return (MemTailer*)tlrPtr;
  }

  static MemHeader* getHeader(void* ptr) {
    auto p = (char*)ptr;
    p = p - utils::roundToAlignment(sizeof(MemHeader), alignment());
    return (MemHeader*)p;
  }

  static MemHeader* getHeader(Block blk) { return (MemHeader*)blk.ptr; }

  static Block getBlock(void* ptr) {
    MemHeader* hdr =
        (MemHeader*)(((char*)ptr) -
                     utils::roundToAlignment(sizeof(MemHeader), alignment()));
    return {hdr, hdr->size};
  }

  static void* getDataBegin(Block blk) {
    return (char*)blk.ptr +
           utils::roundToAlignment(sizeof(MemHeader), alignment());
  }

  static constexpr size_t goodSize(size_t size) {
    return utils::roundToAlignment(sizeof(MemHeader), alignment()) +
           utils::roundToAlignment(size + sizeof(MemTailer), alignment());
  }

  static constexpr size_t dataSize(Block blk) {
    return blk.size - (utils::roundToAlignment(sizeof(MemHeader), alignment()) +
                       sizeof(MemTailer));
  }

  static constexpr size_t alignment() { return 16; }
};

#endif
