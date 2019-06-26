# SWIG

SWIG is an interface compiler that connects programs written in C and C++ with scripting languages. It works by taking the declarations found in C/C++ header files and using them to generate the wrapper code that scripting languages need to access the underlying C/C++ code. <br>

Download SWIG from [their site](http://www.swig.org/download.html) and follow steps given [here](https://www.dev2qa.com/how-to-install-swig-on-macos-linux-and-windows/) to complete installation. <br>

### Note : if you get swig: command not found error then go to /etc/ and source profile so that the environment variables are set.

The steps explained are for python.

“wrapper” functions are needed to access C/C++ which serve as a glue layer between languages.

After writing the .c and .h files for the C library, do this: 

1. `$ swig -python example.i`

2. ` $ gcc -c example.c example_wrap.c \` <br>
   `-I/usr/bin/python2.7`<br>
   This is because the python2.7 file is found in /usr/bin/ on the BeagleBone Black Debian

3. On step #2, you might encounter this error:
   ```
   fatal error: Python.h: No such file or directory
   # include <Python.h>
                     ^
   compilation terminated.
   ```
   Following might be possible causes –
   
   You might not have ‘Python.h’ file or
   You are providing wrong location of ‘Python.h’ file to compiler
   
   To get ‘Python.h’ You must install Python-dev using following command – <br>
   
   `$ sudo apt-get install python-dev`<br>
   Most probably this will already be installed on the BBB.<br>
   
   To find the correct path of ‘Python.h’ execute following command –<br>
   `$ python-config --cflags`<br>
   The output will be something like this - 

   _**-I/usr/include/python2.7** -I/usr/include/arm-linux-gnueabihf/python2.7  -fno-strict-aliasing -Wdate-time -D_FORTIFY_SOURCE=2 -g -fdebug-prefix-map=/build/python2.7-EEvVvy/python2.7-2.7.13=. -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes_ <br>

   **I'm using g++ instead of gcc because gcc isn't allowing dynamic memory allocation in my program**<br>

   Now do `$ g++ -c -fpic example_wrap.c example.c -I/usr/include/python2.7` <br>
   I thought I will have to provide -I/usr/bin/python2.7 but it clearly is -I/usr/include/python2.7<br>

   This should remove the "Python.h" error and will generate **_example_wrap.o_** and **_example.o_** files.<br>

4. Now, at last, we have to link generated objects files together to create a shared object which is analogous to dll files in windows. <br>`$ g++ -shared example.o example_wrap.o -o _example.so`: this will generate a “example.so” shared object file.(**Note:** Sometimes `gcc` will be needed instead of `g++`)

5. Now we are ready to test out python wrapper by importing it. Make sure you are in directory having this wrapper file.

