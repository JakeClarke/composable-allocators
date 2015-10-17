
#include "bitmapalloc.h"
#include "segregatedalloc.h"
#include "fallbackalloc.h"
#include "threadlocalalloc.h"
#include "freelistalloc.h"
#include "nullalloc.h"
#include "mallocator.h"

#include <iostream>


struct testStruct
{
	char myThing[124];
};

#define BIG (4*1024)
#define BLOCKSIZE (4*1024)
#define NUM_BLOCKS (1024)

SegregatedAlloc<
	FallBackAlloc<ThreadLocalAlloc<FreeListAlloc<1024>>,BitMapAlloc<Mallocator,BLOCKSIZE, NUM_BLOCKS>>, NullAlloc,
	1024> myAlloc;

int main(int argc, char const *argv[]){
	auto blk = myAlloc.allocate(sizeof(testStruct));
	if (blk.ptr == nullptr) {
		std::cout << "did not allocate" << std::endl;
	} else {
		std::cout << "allocated: " << std::hex << blk.ptr << ", size:" << blk.size << " bytes."<< std::endl;
	}
	return 0;
}