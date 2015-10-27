#include "../include/compalloc/bitmapalloc.h"
#include "../include/compalloc/mallocator.h"

#include <gtest/gtest.h>

#define BLOCKSIZE (4 * 1024)
#define NUM_BLOCKS (256)

using namespace compalloc;

using AllocToTest = BitMapAlloc<Mallocator,BLOCKSIZE,NUM_BLOCKS>;

TEST(BitMapAlloc, allocate) {
	const auto size = sizeof(char) * 100;

	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);
}


TEST(BitMapAlloc, deallocate) {
	const auto size = sizeof(char) * 100;

	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	myAlloc.deallocate(blk);
}

TEST(BitMapAlloc, reallocate) {
	const auto size = sizeof(char) * BLOCKSIZE;

	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	const auto delta = sizeof(char) * BLOCKSIZE;
	ASSERT_TRUE(myAlloc.reallocate(blk, delta));
}

TEST(BitMapAlloc, owns) {
	const auto size = sizeof(char) * 100;

	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	ASSERT_TRUE(myAlloc.owns(blk));
}