#ifndef _H_ALLOC_COMMON
#define _H_ALLOC_COMMON

#include <cstddef>

struct Block {
	void * ptr;
	size_t size;
};

#endif