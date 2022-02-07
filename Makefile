all: preprocess encode

preprocess: preprocess.o
	g++ preprocess.o -o preprocess

encode: encode.o
	g++ encode.o -o encode

preprocess.o: preprocess.cpp defns.h
	g++ -c preprocess.cpp

encode.o: encode.cpp defns.h
	g++ -c encode.cpp