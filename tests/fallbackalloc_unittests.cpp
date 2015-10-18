#include "../mallocator.h"
#include "../fallbackalloc.h"
#include "../nullalloc.h"

#include <gtest/gtest.h>

using AllocToTest = FallBackAlloc<NullAlloc,Mallocator>;

TEST(fallback, allocate) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);

	// leaky
}

TEST(fallback, reallocate) {
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

TEST(fallback, owns) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);
	
	ASSERT_TRUE(myAlloc.owns(blk));
	// leaky
}

TEST(fallback, goodSize) {
	const auto s = sizeof(char) * 100;

	// nullalloc will always return 0.
	ASSERT_GE(AllocToTest::goodSize(s), 0);
}