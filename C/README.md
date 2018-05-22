# Swig
Swig is a tool to generate language bindings. It can be used to create interfaces for C/C++ code in various languages. 
## Requirements
* Swig (v3.0 or newer)
* Python-3.5
## Steps to generate python bindings
```
swig -python pruss.i
gcc -c -fpic pruss.c pruss_wrap.c -I/usr/include/python3.5 
gcc -shared pruss.o pruss_wrap.o -o _pruss.so
```
