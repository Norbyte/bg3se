# Final names of binaries
EXECUTABLE = Bin/zipsample
SO_LIBRARY = Bin/libzip.so

# C & C++ compiler
#CC       = gcc
#CXX      = g++-4.8
CC        = clang
CXX       = clang++
CFLAGS    = -fPIC -Wno-enum-conversion -O3
CXXFLAGS  = -fPIC -std=c++11 -O3

# Linker flags
LDFLAGS   = -pthread

# Sources of external libraries
SRC_ZLIB  = $(wildcard Source/ZipLib/extlibs/zlib/*.c)
SRC_LZMA  = $(wildcard Source/ZipLib/extlibs/lzma/unix/*.c)
SRC_BZIP2 = $(wildcard Source/ZipLib/extlibs/bzip2/*.c)

# ZipLib sources
SRC = \
		$(wildcard Source/ZipLib/*.cpp)        \
		$(wildcard Source/ZipLib/detail/*.cpp)

# Object files			
OBJS = \
		$(SRC:.cpp=.o)	   \
		$(SRC_ZLIB:.c=.o)  \
		$(SRC_LZMA:.c=.o)  \
		$(SRC_BZIP2:.c=.o)

# Rules
all: $(EXECUTABLE) $(SO_LIBRARY)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) Source/Sample/Main.cpp -o $@ $^

$(SO_LIBRARY): $(OBJS)
	$(CXX) $(LDFLAGS) -shared -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf `find Source -name '*.o'` ziplib.tar.gz Bin/*.zip Bin/out* $(EXECUTABLE) $(SO_LIBRARY)

tarball:
	tar -zcvf ziplib.tar.gz *
	