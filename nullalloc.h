#ifndef _H_NULLALLOC
#define _H_NULLALLOC

#include "alloc-common.h"
#include <cassert>

class NullAlloc
{
public:
	Block allocate(size_t) {
		return {nullptr, 0};
	}

	void deallocate(Block) {
		assert(false);
	}
	
	bool owns(Block) {
		return false;
	}

	constexpr static size_t goodSize(size_t) {
		return 0;
	}

};

#endif