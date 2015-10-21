#ifndef _H_MALLOCATOR
#define _H_MALLOCATOR

#include "alloc-common.h"
#include <cstdlib>

class Mallocator {
 public:
  Block allocate(size_t size) {
    auto ptr = malloc(size);
    if (ptr == nullptr) {
      return {nullptr, 0};
    }

    return {ptr, size};
  }

  void deallocate(Block blk) { free(blk.ptr); }

  bool owns(Block) { return true; }

  bool reallocate(Block &blk, size_t delta) {
  	const auto newSize = blk.size + delta;
  	const auto ptr = realloc(blk.ptr, newSize);
  	if (ptr != nullptr) {
  		blk = {ptr, newSize};
  		return true;
  	}
  	return false;
  }

  static constexpr size_t goodSize(size_t size) { return size; }
};

#endif