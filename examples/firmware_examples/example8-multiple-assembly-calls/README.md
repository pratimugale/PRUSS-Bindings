# Calling multiple .asm files from C-program

## Some background details:

1. The `clpru` compiler uses C/C++ code to generate assembly language code.
2. The assembler built into `clpru` translates assembly language code into machine object (.object) files.
3. The `lnkpru` linker combines the object files into a single executable object file (.out). 
4. The generated `.out` file can be executed directly on a PRU device. To do so on a BB, it must be renamed to `am335x-pru0-fw` or `am335x-pru1-fw` and placed in the `/lib/firmware` directory. 
5. On starting the PRU, it will be loaded with the respective firmware in this `/lib/firmware` directory.

