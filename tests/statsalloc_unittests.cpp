#include "../include/compalloc/statsalloc.h"

#include "../include/compalloc/stackallocator.h"

#include <gtest/gtest.h>

using namespace compalloc;

TEST(StatsAlloc, allocate) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	EXPECT_NE(nullptr, blk.ptr);
	EXPECT_GE(blk.size, testSize);
}

TEST(StatsAlloc, deallocate) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	EXPECT_NE(nullptr, blk.ptr);
	EXPECT_GE(blk.size, testSize);

	myAlloc.deallocate(blk);
}

TEST(StatsAlloc, requestedSize) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, testSize);

	auto stats = myAlloc.getStats();
	EXPECT_EQ(testSize, stats.requested);
}

TEST(StatsAlloc, actualSize) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, testSize);

	auto stats = myAlloc.getStats();
	EXPECT_EQ(blk.size, stats.allocated);
}

TEST(StatsAlloc, deallocatedSize) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, testSize);
	myAlloc.deallocate(blk);

	auto stats = myAlloc.getStats();
	EXPECT_EQ(blk.size, stats.deallocated);
}

TEST(StatsAlloc, currentSize) {
	using AllocToTest = StatsAlloc<StackAllocator<1024>>;
	constexpr size_t testSize = 100 * sizeof(char);
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(testSize);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, testSize);

	auto stats = myAlloc.getStats();
	EXPECT_EQ(blk.size, stats.current);
}