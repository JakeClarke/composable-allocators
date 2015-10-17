#ifndef _H_BIT_MAP_ALLOC
#define _H_BIT_MAP_ALLOC

#include "alloc-common.h"
#include <cstdint>
#include <cassert>
#include <mutex>

template<class backing, size_t blockSize, size_t numBlocks>
class BitMapAlloc: private backing {
private:
	Block root;
	std::mutex mapMutex;
	uint8_t map[numBlocks/8];

	void markFree(size_t num) {
		assert(isUsed(num));

		map[getMapByteNum(num)] ^= getMapBit(num);
	}

	void markUsed(size_t num) {

		map[getMapByteNum(num)] |= getMapBit(num);
	}

	bool isUsed(size_t num) {

		return map[getMapByteNum(num)] & getMapBit(num);
	}

	void * getBlkPtr(size_t blkNum) {
		return (void*)((char *)root.ptr + (blkNum * blockSize));
	}

	size_t getBlkNum(void * ptr) {
		return ((size_t)(((char *)ptr) - root.ptr))/ blockSize;
	}

	static constexpr size_t getMapByteNum(size_t num) {
		return num / 8;
	}

	static constexpr size_t getMapBit(size_t num) {
		return (1 << (8-(num % 8)));
	}

public:
	BitMapAlloc():root(backing::allocate(numBlocks*blockSize)) {
		assert(root.ptr != nullptr);
	}

	Block allocate(size_t size) {
		std::lock_guard<std::mutex> lock(mapMutex);
		const auto blocksNeeded = goodSize(size)/blockSize;

		for(size_t start = 0; start<numBlocks; start++) {
			if (!isUsed(start))
			{
				size_t freeCount = 0;
				for(size_t end = start; (end < numBlocks) && (freeCount < blocksNeeded); end++) {
					if (isUsed(end))
					{
						break;
					}
					freeCount++;
				}
				if (freeCount == blocksNeeded) {
					for (size_t i = (start - (size_t)root.ptr); i < start + blocksNeeded; i++) {
						markUsed(i);
					}
					return {getBlkPtr(start), goodSize(size)};
				}
			}
		}

		return {nullptr, 0};
	}

	void deallocate(Block blk) {
		std::lock_guard<std::mutex> lock(mapMutex);
		for(size_t i = getBlkNum(blk); i < getBlkNum(blk) + (blk.size / blockSize); i++) {
			markFree(i);
		}
	}

	constexpr static size_t alignment() {
		return 16;
	}

	static_assert(blockSize % alignment() == 0, "Blocks must be aligned");

	constexpr static size_t goodSize(size_t a) {
		auto rem = a % blockSize;
		return a  + (blockSize - rem);
	}

	bool owns(Block blk) {
		if (((char *)blk.ptr - root.ptr) % blockSize == 0) {
			return true;
		}
		return false;
	}

	bool reallocate(Block &blk, size_t delta) {
		const auto neededSize = goodSize(blk.size+delta);
		const auto blocksNeeded = neededSize/blockSize;

		return false;

	}

	~BitMapAlloc(){
		if (root.ptr != nullptr) {
			backing::deallocate(root);
			root = {nullptr, 0};
		}
	}
};

#endif