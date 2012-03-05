CXX=g++
CC=gcc
CXXFLAGS=-I../include/art -Wall -Wextra 
CFLAGS=-I../include/art -Wall -Wextra
LFLAGS=-L . -l:../release/libartsim.so -lm

all: ../release/libartsim.so ../release/test_cpp  ../release/test_c
	@echo "Please execute \"export LD_LIBRARY_PATH=.:\$$LD_LIBRARY_PATH\" before executing the test programs!"

../release/test_cpp: testlibart.cpp Makefile
	$(CXX) -o $@ $(CXXFLAGS) $< $(LFLAGS)

../release/test_c: testlibart.c Makefile
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	rm ../release/test_cpp ../release/test_c
