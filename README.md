# Convertor

Code written in the framework of a three-month internship in TU Delft about the parallelisation of C programs. This Linux software convert a C code and an MXIF file into a parallelised C++ code using the *XPU* library.

**Requirements**

This software requires at least the *Rose compiler* library to execute. MXIF files are generated thanks to the *MCPROF* application.To compile and execute the generated code, the *XPU* library is necessary. These tools are available here:

- [*Rose compiler*](http://rosecompiler.org/) (the use of a virtual machine is the easiest and recommanded way)
- [*MCPROF*](https://bitbucket.org/imranashraf/mcprof)
- [*XPU*](http://www.xpu-project.net/en/index.php)

**How to use Convertor**

You can compile the software thanks to the classic `make` command.

With `make test` or directly with the bash script `test.sh` you can test Convertor on the test cases in the `testMxif` folder. According to the arguments given to `test.sh` more or less will be done.

- `test.sh g` will generate the C++/XPU files.
- `test.sh c` will compile the C++/XPU files.
- `test.sh x` will execute the generated C++/XPU files and the original C files.

All the arguments can be combined, seperated by spaces. By default, everything is done. Information about the generation is written in a `convertor.log` file.

To use Convertor on you own files, follow the syntax `./Convertor inputfile.c inputfile.mxif outputfile.cpp`.

`Makefile` and the script `test.sh` are easily readable and customizable.
