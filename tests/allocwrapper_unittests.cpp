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