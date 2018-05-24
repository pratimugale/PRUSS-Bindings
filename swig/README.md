# Swig
Swig is a tool to generate language bindings. It can be used to create interfaces for C/C++ code in various languages. 
## Description
* pruss.c - The C code which the bindings actually correspond to.
* pruss.i - The interface file which swig uses to generate the wrapper code.

### Instructions to generate bindings
```
swig -python pruss.i
gcc -c -fpic pruss.c pruss_wrap.c -I/usr/include/python3.5 
gcc -shared pruss.o pruss_wrap.o -o _pruss.so
```
