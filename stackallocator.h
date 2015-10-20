#ifndef _H_STACKALLOCATOR
#define _H_STACKALLOCATOR

#include "alloc-common.h"

template <size_t stackSize>
class StackAllocator {
public:
	StackAllocator():current(0) {

	}

	Block allocate(size_t size) {
		if (current + goodSize(size) <= sizeof(char) * stackSize)
		{
			auto ptr = stack + current;
			current += goodSize(size);
			return {ptr, goodSize(size)};
		}
		return {nullptr, 0};
	}

	void deallocate(Block blk) {
		if(lastAllocation(blk)) {
			current -= blk.size;
		}
	}

	bool owns(Block blk) {
		char *ptr = (char *)blk.ptr;
		if (ptr >= stack && ptr < stack + stackSize) {
			return true;
		}
		return false;
	}

	bool reallocate(Block &blk, size_t delta) {
		const auto needDelta = goodSize(blk.size + delta) - blk.size;
		if(lastAllocation(blk) &&
			current + needDelta <= stackSize) {
				current += needDelta;
				blk.size = goodSize(blk.size + delta);
				return true;
		}
		return false;
	}

	static constexpr size_t goodSize(size_t size) {
		return size + (allignment() - (size % allignment()));
	}

	static constexpr size_t allignment() {
		return 16;
	}

	static_assert(stackSize % allignment() == 0, "StackSize must be alligned");
private:
	bool lastAllocation(Block blk) {
		if (blk.ptr == stack + (current - blk.size)) {
			return true;
		}

		return false;
	}

	char stack[stackSize];
	size_t current;

};

#endif