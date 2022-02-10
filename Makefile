all: preprocess encode decode

preprocess: preprocess.o
	g++ preprocess.o -o preprocess

encode: encode.o
	g++ encode.o -o encode

decode: decode.o
	g++ decode.o -o decode

preprocess.o: preprocess.cpp defns.h
	g++ -c preprocess.cpp

encode.o: encode.cpp defns.h
	g++ -c encode.cpp

decode.o: decode.cpp defns.h
	g++ -c decode.cpp