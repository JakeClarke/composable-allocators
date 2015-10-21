#include "../threadlocalalloc.h"
#include "../stackallocator.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

using AllocToTest = ThreadLocalAlloc<StackAllocator<1024>>;

TEST(ThreadLocalAlloc, allocate) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);
}

TEST(ThreadLocalAlloc, deallocate) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	myAlloc.deallocate(blk);
}

TEST(ThreadLocalAlloc, owns) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	ASSERT_TRUE(myAlloc.owns(blk));
}

TEST(ThreadLocalAlloc, reallocate) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	auto blk = myAlloc.allocate(size);
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);

	const auto delta = sizeof(char) * 100;
	EXPECT_LT(blk.size, size + delta);
	ASSERT_TRUE(myAlloc.reallocate(blk, delta));
	ASSERT_GE(blk.size, size+delta);
}

TEST(ThreadLocalAlloc, doesntOwn) {
	AllocToTest myAlloc;
	ASSERT_FALSE(myAlloc.owns({nullptr, 0}));
}

TEST(ThreadLocalAlloc, doesntOwnAcrossThreads) {
	const auto size = sizeof(char) * 100;
	AllocToTest myAlloc;
	Block blk = {nullptr, 0};

	std::thread t2([&]() {
		blk = myAlloc.allocate(size);
	});

	t2.join();
	ASSERT_NE(nullptr, blk.ptr);
	ASSERT_GE(blk.size, size);
	ASSERT_FALSE(myAlloc.owns(blk));
}