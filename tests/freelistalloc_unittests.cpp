#include "../include/compalloc/freelistalloc.h"
#include "../include/compalloc/mallocator.h"

#include <gtest/gtest.h>

using namespace compalloc;

struct testStruct
{
	char thing[100];
};

TEST(FreeListAlloc, allocate) {
	using AllocToTest = FreeListAlloc<Mallocator>;
	AllocToTest myAlloc;

	auto blk = myAlloc.allocate(sizeof(testStruct));
	ASSERT_NE(blk.ptr, nullptr);
	ASSERT_GE(blk.size, sizeof(testStruct));
}

TEST(FreeListAlloc, deallocate) {
	using AllocToTest = FreeListAlloc<Mallocator>;
	AllocToTest myAlloc;

	auto blk = myAlloc.allocate(sizeof(testStruct));
	ASSERT_NE(blk.ptr, nullptr);
	ASSERT_GE(blk.size, sizeof(testStruct));
	myAlloc.deallocate(blk);
}

TEST(FreeListAlloc, sameback) {
	using AllocToTest = FreeListAlloc<Mallocator>;
	constexpr size_t allocSize = sizeof(testStruct);
	AllocToTest myAlloc;

	auto first = myAlloc.allocate(allocSize);
	ASSERT_NE(first.ptr, nullptr);
	ASSERT_GE(first.size, sizeof(allocSize));
	myAlloc.deallocate(first);

	auto second = myAlloc.allocate(allocSize);
	EXPECT_EQ(first.ptr, second.ptr);
	EXPECT_EQ(first.size, second.size);
}

TEST(FreeListAlloc, goodSize) {
	using AllocToTest = FreeListAlloc<Mallocator>;
	constexpr size_t allocSize = sizeof(testStruct);
	ASSERT_GE(allocSize, AllocToTest::goodSize(allocSize));
}