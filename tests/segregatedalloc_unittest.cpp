#include "../mallocator.h"
#include "../segregatedalloc.h"
#include "../nullalloc.h"

#include <gtest/gtest.h>

const size_t threshold = sizeof(char) * 1024;
const size_t bigThing = sizeof(char) * 2000;
const size_t smallThing = sizeof(char) * 20;

using AllocBig = SegregatedAlloc<NullAlloc,Mallocator, threshold>;
using AllocSmall = SegregatedAlloc<Mallocator, NullAlloc, threshold>;

TEST(segregatedBig, allocate) {
	AllocBig myAlloc;
	auto blk = myAlloc.allocate(bigThing);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, bigThing);

	// leaky
}

TEST(segregatedBig, allocatefail) {
	AllocBig myAlloc;
	auto blk = myAlloc.allocate(smallThing);
	ASSERT_EQ(blk.ptr, nullptr);
	EXPECT_EQ(blk.size, 0);

	// leaky
}

TEST(segregatedSmall, allocate) {
	AllocSmall myAlloc;
	auto blk = myAlloc.allocate(smallThing);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, smallThing);
}

TEST(segregatedSmall, allocatefail) {
	AllocSmall myAlloc;
	auto blk = myAlloc.allocate(bigThing);
	ASSERT_EQ(blk.ptr, nullptr);
	EXPECT_EQ(blk.size, 0);

	// leaky
}

TEST(segregatedSmall, reallocate) {
	AllocSmall myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);

	const auto delta = sizeof(char) * 100;
	ASSERT_TRUE(myAlloc.reallocate(blk, delta));
	ASSERT_NE(blk.ptr, nullptr);
	ASSERT_GE(blk.size, expectedSize + delta);
}

TEST(segregatedSmall, owns) {
	AllocSmall myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);
	
	ASSERT_TRUE(myAlloc.owns(blk));
	// leaky
}

TEST(segregatedSmall, goodSize) {
	const auto s = sizeof(char) * 100;

	// nullalloc will always return 0.
	ASSERT_GE(AllocSmall::goodSize(s), 0);
}