#ifndef _H_FALLBACK_ALLOC
#define _H_FALLBACK_ALLOC

#include "alloc-common.h"

template <typename primary, typename secondary>
class FallBackAlloc: private primary, private secondary
{
public:
	public:
	Block allocate(size_t size) 
  { 
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

  void owns(Block blk) {
  	return primary::owns(blk) || secondary::owns(blk);
  }

  constexpr static size_t goodSize(size_t size) {
  	return primary::goodSize(size);
  }
};

#endif