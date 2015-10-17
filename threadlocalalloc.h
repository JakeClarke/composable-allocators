#ifndef _H_THREAD_LOCAL_ALLOC
#define _H_THREAD_LOCAL_ALLOC

template<typename threadlocalalloc>
class ThreadLocalAlloc
{
public:
	Block allocate(size_t size) {
		return tll.allocate(size);
	}

	void deallocate(Block blk) {
		tll.deallocate(blk);
	}

	bool owns(Block blk) {
		return tll.owns(blk);
	}

	static constexpr size_t goodSize(size_t size) {
		return tll.goodSize(size);
	}
private:
	static thread_local threadlocalalloc tll;
};

template<typename t> thread_local t ThreadLocalAlloc<t>::tll;

#endif