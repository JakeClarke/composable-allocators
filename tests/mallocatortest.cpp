// malloc test
#include "../mallocator.h"

#include <gtest/gtest.h>

using AllocToTest = Mallocator;

TEST(mallacator, allocate) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);

	// leaky
}

TEST(mallacator, reallocate) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);

	const auto delta = sizeof(char) * 100;
	ASSERT_TRUE(myAlloc.reallocate(blk, delta));
	ASSERT_NE(blk.ptr, nullptr);
	ASSERT_GE(blk.size, expectedSize + delta);
}

TEST(mallacator, owns) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);
	
	ASSERT_TRUE(myAlloc.owns(blk));
	// leaky
}

TEST(mallacator, goodSize) {
	const auto s = sizeof(char) * 100;

	ASSERT_GE(AllocToTest::goodSize(s), s);
}