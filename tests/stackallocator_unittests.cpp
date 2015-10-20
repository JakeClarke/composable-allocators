#include "../stackallocator.h"

#include <gtest/gtest.h>

using AllocToTest = StackAllocator<1024>;

TEST(StackAllocator, allocate) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);
}

TEST(StackAllocator, deallocate) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);
}

TEST(StackAllocator, owns) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	ASSERT_TRUE(myAlloc.owns(blk));
}