#ifndef _H_FREE_LIST_ALLOC
#define _H_FREE_LIST_ALLOC

#include "alloc-common.h"

template <size_t numSlots>
class FreeListAlloc {
 public:
  Block allocate(size_t size) {
    Block blk = {nullptr, 0};
    if (usedSlots == 0) {
      return blk;
    }

    for (size_t i = 0; i < numSlots; ++i) {
      if (slots[i].size == size) {
      	auto temp = slots[i];
        blk = slots[i];
        slots[i] = temp;
        usedSlots--;
        break;
      }
    }

    return blk;
  }

  void deallocate(Block blk) {
    //assert(usedSlots < numSlots);

    for (size_t i = 0; i < numSlots; i++) {
      if (slots[i].ptr == nullptr) {
        slots[i] = blk;
        usedSlots++;
        return;
      }
    }
  }

  bool reallocate(Block &blk, size_t delta) {
  	return false;
  }

  bool owns(Block) { return usedSlots < numSlots; }

  constexpr static size_t goodSize(size_t size) { return size; }

 private:
  Block slots[numSlots];
  size_t usedSlots;
};;

#endif