#include "../allocwrapper.h"
#include "../mallocator.h"

#include <gtest/gtest.h>

using AllocToTest = AllocWrapper<Mallocator>;

TEST(AllocWrapper, allocate) {
	AllocToTest myAlloc;
	const size_t size = sizeof(char) * 100;
	ASSERT_NE(myAlloc.allocate(size), nullptr);
}

TEST(AllocWrapper, deallocate) {
	AllocToTest myAlloc;
	const size_t size = sizeof(char) * 100;
	const auto ptr = myAlloc.allocate(size);
	ASSERT_NE(ptr, nullptr);

	myAlloc.deallocate(ptr);
}

TEST(AllocWrapper, goodSize) {
	const size_t size = sizeof(char) * 100;
	ASSERT_GE(AllocToTest::goodSize(size), size);
}

TEST(AllocWrapper, goodSizeAligned) {
	const size_t size = sizeof(char) * 100;
	ASSERT_EQ(0, AllocToTest::goodSize(size) % AllocToTest::alignment());
}

#define TAG_STR "test-tag"

TEST(AllocWrapper, setsTag) {
	size_t size = 100 * sizeof(char);

	AllocToTest myAlloc;
	auto ptr = myAlloc.allocate(size, TAG_STR);
	ASSERT_NE(nullptr, ptr);

	auto tlr = AllocToTest::getTailer(ptr);

	ASSERT_STREQ(TAG_STR, tlr->tag);
}

TEST(AllocWrapper, blockRecoverable) {
	const size_t size = 100 * sizeof(char);
	const size_t allocSize = AllocToTest::goodSize(size);

	AllocToTest myAlloc;
	auto ptr = myAlloc.allocate(size, TAG_STR);
	ASSERT_NE(nullptr, ptr);

	auto blk = AllocToTest::getBlock(ptr);
	ASSERT_EQ(allocSize, blk.size);
}

TEST(AllocWrapper, dataStart) {
	const size_t size = 100 * sizeof(char);
	const size_t allocSize = AllocToTest::goodSize(size);

	AllocToTest myAlloc;
	auto ptr = myAlloc.allocate(size, TAG_STR);
	ASSERT_NE(nullptr, ptr);

	auto blk = AllocToTest::getBlock(ptr);
	ASSERT_EQ(ptr, AllocToTest::getDataBegin(blk));
}

TEST(AllocWrapper, reallocate) {
	const size_t size = 100 * sizeof(char);
	const size_t allocSize = AllocToTest::goodSize(size);

	AllocToTest myAlloc;
	auto ptr = myAlloc.allocate(size);
	ASSERT_NE(nullptr, ptr);
	auto blk = AllocToTest::getBlock(ptr);

	auto newAllocSize = allocSize * 2;
	auto newPtr = myAlloc.reallocate(ptr, newAllocSize);
	ASSERT_NE(nullptr, newPtr);

	auto newBlk = AllocToTest::getBlock(ptr);
	ASSERT_GE(newBlk.size, newAllocSize);

}

TEST(AllocWrapper, owns) {
	// This is a bit of a non test.
	// The Mallocator will always say true.
	// TODO: Use a different backing allocator.

	AllocToTest myAlloc;
	const size_t size = sizeof(char) * 100;
	auto ptr = myAlloc.allocate(size);
	ASSERT_NE(ptr, nullptr);

	ASSERT_TRUE(myAlloc.owns(ptr));
}