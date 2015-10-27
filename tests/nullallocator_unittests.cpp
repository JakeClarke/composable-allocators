// malloc test
#include "../include/compalloc/nullalloc.h"

#include <gtest/gtest.h>

using namespace compalloc;

using AllocToTest = NullAlloc;


TEST(nullalloc, allocate) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_EQ(blk.ptr, nullptr);
	EXPECT_EQ(blk.size, 0);

	// leaky
}

TEST(nullalloc, owns) {
	AllocToTest myAlloc;
	const auto expectedSize = sizeof(char) * 100;
	auto blk = myAlloc.allocate(expectedSize);
	ASSERT_EQ(blk.ptr, nullptr);
	EXPECT_EQ(blk.size, 0);
	
	ASSERT_FALSE(myAlloc.owns(blk));
	// leaky
}

TEST(nullalloc, doesNotOwn) {
	AllocToTest myAlloc;

	Block blk = {nullptr, 0};
	
	ASSERT_FALSE(myAlloc.owns(blk));
	// leaky
}

TEST(nullalloc, goodSize) {
	const auto s = sizeof(char) * 100;

	ASSERT_EQ(AllocToTest::goodSize(s), s);
}
