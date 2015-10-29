#include "../include/compalloc/bitmapalloc.h"
#include "../include/compalloc/mallocator.h"

#include <gtest/gtest.h>

constexpr size_t blockSize = 4 * 1024;
constexpr size_t numBlocks = 256;

using namespace compalloc;

using AllocToTest = BitMapAlloc<Mallocator,blockSize,numBlocks>;

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
	const auto size = sizeof(char) * blockSize;

	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	const auto delta = sizeof(char) * blockSize;
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