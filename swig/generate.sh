#!/bin/sh

if [[ $1 = "-python" ]]; 
then
	swig -c++ -python pruss.i 
	g++ -fpic -c pruss.h pruss_wrap.cxx pruss.cpp -I/usr/include/python3.5/
	gcc -shared pruss_wrap.o pruss.o -o _pruss.so -lstdc++

	echo "python module generated successfully"

elif [[ $1 = "-nodejs" ]];
then
	swig -javascript -node -c++ pruss.i
	node-gyp configure build
	echo "node module generated succesfully in: ./build/Release/pruss.node"

else
	echo "Specify language: -python | -nodejs"

fi
