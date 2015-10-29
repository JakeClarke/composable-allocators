#ifndef _H_ALLOCWRAPPER
#define _H_ALLOCWRAPPER

#include "alloc-common.h"

#include <cstring>

namespace compalloc {

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
    auto blk = alloc::allocate(goodSize(size));
    if (blk.ptr == nullptr) {
      return nullptr;
    }

    putMetaData(blk, tag);

    // return the start of the allocation.
    return getDataBegin(blk);
  }

  void* allocate(size_t size) { return allocate(size, nullptr); }

  void deallocate(void* ptr) { alloc::deallocate(getBlock(ptr)); }

  void* reallocate(void* ptr, size_t size) {
    auto blk = getBlock(ptr);
    if (blk.size >= goodSize(size)) {
      return getDataBegin(blk);
    }

    auto tlr = getTailer(ptr);
    if (alloc::reallocate(blk, size - blk.size)) {
      putMetaData(blk, tlr->tag);
      return getDataBegin(blk);
    }
    return nullptr;
  }

  bool owns(void* ptr) { return alloc::owns(getBlock(ptr)); }

  static MemTailer* getTailer(void* ptr) {
    const size_t tailOffset =
        getHeader(ptr)->size -
        (sizeof(MemTailer) +
         utils::roundToAlignment(sizeof(MemHeader), alignment));
    auto tlrPtr = static_cast<char*>(ptr) + tailOffset;
    return reinterpret_cast<MemTailer*>(tlrPtr);
  }

  static MemTailer* getTailer(Block blk) {
    auto tlrPtr = static_cast<char*>(blk.ptr) + (blk.size - sizeof(MemTailer));
    return reinterpret_cast<MemTailer*>(tlrPtr);
  }

  static MemHeader* getHeader(void* ptr) {
    auto p = static_cast<char*>(ptr);
    p = p - utils::roundToAlignment(sizeof(MemHeader), alignment);
    return reinterpret_cast<MemHeader*>(p);
  }

  static MemHeader* getHeader(Block blk) { return (MemHeader*)blk.ptr; }

  static Block getBlock(void* ptr) {
    MemHeader* hdr = reinterpret_cast<MemHeader*>(
        static_cast<char*>(ptr) -
        utils::roundToAlignment(sizeof(MemHeader), alignment));
    return {hdr, hdr->size};
  }

  static void* getDataBegin(Block blk) {
    return static_cast<char*>(blk.ptr) +
           utils::roundToAlignment(sizeof(MemHeader), alignment);
  }

  static constexpr size_t goodSize(size_t size) {
    return alloc::goodSize(
        utils::roundToAlignment(sizeof(MemHeader), alignment) + size +
        sizeof(MemTailer));
  }

  static constexpr size_t dataSize(Block blk) {
    return blk.size - (utils::roundToAlignment(sizeof(MemHeader), alignment) +
                       sizeof(MemTailer));
  }

  static constexpr size_t alignment = alloc::alignment;

 private:
  static void putMetaData(const Block blk, const char* tag) {
    MemHeader* hdr = getHeader(blk);
    hdr->size = blk.size;

    MemTailer* tlr = getTailer(blk);
    memset(tlr, 0, sizeof(*tlr));
    if (tag != nullptr) {
      std::strncpy(tlr->tag, tag, TAILER_TAG_SIZE - 1);
    }
  }
};
}  // compalloc

#endif
