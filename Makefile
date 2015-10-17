CXXFLAGS =	-O3 -g -Wall -Wextra -Wcast-align -pedantic -fmessage-length=0 -std=c++11

OBJS =	alloc-tests.o

TARGET =	alloc-tests

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
