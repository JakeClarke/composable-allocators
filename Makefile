# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

OBJS =	alloc-tests.o

GTEST_DIR = googletest/googletest

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

TARGET =	alloc-tests

TESTS = mallocator_unittests nullallocator_unittests fallbackalloc_unittests segregatedalloc_unittest allocwrapper_unittests stackallocator_unittests

TESTS_DIR = tests

# Target

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

test: $(TESTS)

all:	$(TARGET) test

.PHONEY: clean

clean:
	rm -f $(TARGET) $(TESTS) *.o gtest_main.a

# Gtest stuff
gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

## My tests
# malloc tests
mallocator_unittests.o: $(TESTS_DIR)/mallocator_unittests.cpp mallocator.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/mallocator_unittests.cpp

mallocator_unittests: gtest_main.a mallocator_unittests.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@

# null alloc tests
nullallocator_unittests.o: $(TESTS_DIR)/nullallocator_unittests.cpp nullalloc.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/nullallocator_unittests.cpp

nullallocator_unittests: gtest_main.a nullallocator_unittests.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@

# fall back tests
fallbackalloc_unittests.o: $(TESTS_DIR)/fallbackalloc_unittests.cpp fallbackalloc.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/fallbackalloc_unittests.cpp

fallbackalloc_unittests: gtest_main.a fallbackalloc_unittests.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@

# segregated tests
segregatedalloc_unittest.o: $(TESTS_DIR)/segregatedalloc_unittest.cpp segregatedalloc.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/segregatedalloc_unittest.cpp

segregatedalloc_unittest: gtest_main.a segregatedalloc_unittest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@

# wrapper tests
allocwrapper_unittests.o: $(TESTS_DIR)/allocwrapper_unittests.cpp allocwrapper.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/allocwrapper_unittests.cpp

allocwrapper_unittests: gtest_main.a allocwrapper_unittests.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@

# stackalloctor tests
stackallocator_unittests.o: $(TESTS_DIR)/stackallocator_unittests.cpp stackallocator.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTS_DIR)/stackallocator_unittests.cpp

stackallocator_unittests: gtest_main.a stackallocator_unittests.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
	./$@