CXX=g++
CC=gcc
CXXFLAGS=-I../../../../include/art -Wall -Wextra 
CFLAGS=-I../../../../include/art -Wall -Wextra
LFLAGS=-Wl,-rpath=../../../../../out/release -L . -l:../../../../../out/release/libartsim.so -lm

all: ../../../../../out/release/libartsim.so test_cpp  test_c

test_cpp: testlibart.cpp Makefile
	$(CXX) -o $@ $(CXXFLAGS) $< $(LFLAGS)

test_c: testlibart.c Makefile
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	rm -f test_cpp test_c
