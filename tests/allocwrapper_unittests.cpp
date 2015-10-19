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
	ASSERT_GE(AllocToTest::goodSize(size) % AllocToTest::alignment(), 0);
}