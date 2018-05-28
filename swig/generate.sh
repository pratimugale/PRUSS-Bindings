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

elif [[ $1 = "-perl" ]];
then
	swig -c++ -perl pruss.i
	g++ -fPIC -c pruss_wrap.cxx -I/usr/lib/arm-linux-gnueabihf/perl/5.24.1/CORE
	g++ -shared pruss.o pruss_wrap.o -lstdc++ -o pruss.so
	echo "perl module generated successfully"

elif [[ $1 = "-lua" ]];
then
	swig -c++ -lua pruss.i
	g++ -fPIC -c pruss_wrap.cxx -o pruss_wrap.o -I/usr/include/lua5.2/
	g++ -fPIC -c pruss.cpp -o pruss.o
	g++ -shared pruss.o pruss_wrap.o -o pruss.so -lstdc++ -I/usr/include/lua5.2/
	echo "lua module generated successfully"

elif [[ $1 = "-ruby" ]];
then
	swig -c++ -ruby pruss.i
	g++ -fPIC -c pruss_wrap.cxx -I/usr/include/ruby-2.3.0 -I /usr/include/arm-linux-gnueabihf/ruby-2.3.0
	g++ -shared pruss.o pruss_wrap.o -o pruss.so
	ruby extconf.rb
	make
	make install
	echo "ruby bindings generated successfully"

elif [[ $1 = "-tcl" ]];
then
	swig -c++ -tcl pruss.i
	g++ -fPIC -c pruss.cpp
	g++ -fPIC -c pruss_wrap.cxx -I/usr/include/tcl8.6/
	g++ -shared pruss.o pruss_wrap.o -o pruss.so
	echo "tcl module generated successfully"
else
	echo "Specify language: -python | -nodejs | -perl | -lua | -ruby | -tcl"

fi
