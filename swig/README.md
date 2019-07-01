# Swig
Swig is a tool to generate language bindings. It can be used to create interfaces for C/C++ code in various languages.
Also have a look at `using_swig.md`

## Description
This directory contains the C++ code along with the Swig interface file to generate bindings for the PRUSS.
* pruss.cpp : Contains the main PRU class along with Methods to interact with a PRU object.
* pruss.h : The header file for the PRU class containing class definitions and include files.
* pruss.i : [python, perl, lua] The Swig interface file which generates a wrapper for the PRU class.
* binding.gyp : [nodejs] A JSON style file which node-gyp uses to generate swig wrapper for nodejs bindings.

## Instructions to generate bindings
### python
```
bash generate.sh -python
```
### nodejs
```
bash generate.sh -nodejs
```
### perl
```
bash generate.sh -perl
```
### lua
```
bash generate.sh -lua
```
### ruby
```
bash generate.sh -ruby
```
### tcl
```
bash generate.sh -tcl
```
