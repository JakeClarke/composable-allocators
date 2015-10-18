// malloc test
#include "../mallocator.h"

#include <gtest/gtest.h>


TEST(mallacator, allocate) {
	Mallocator myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);

	// leaky
}

TEST(mallacator, reallocate) {
	Mallocator myAlloc;
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
	Mallocator myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_NE(blk.ptr, nullptr);
	EXPECT_GE(blk.size, expectedSize);
	
	ASSERT_TRUE(myAlloc.owns(blk));
	// leaky
}