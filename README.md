# Convertor

Code written in the framework of a three-month internship in TU Delft about the parallelisation of C programs. This Linux software convert a C code and an MXIF file into a parallelised C++ code using the *XPU* library.

**Requirements**

This software requires at least the *Rose compiler* library to execute. MXIF files are generated thanks to the *MCPROF* application.To compile and execute the generated code, the *XPU* library is necessary. These tools are available here:

- [*Rose compiler*](http://rosecompiler.org/) (the use of a virtual machine is the easiest and recommanded way)
- [*MCPROF*](https://bitbucket.org/imranashraf/mcprof)
- [*XPU*](http://www.xpu-project.net/en/index.php)

**How to use Convertor**

You can compile the software thanks to the classic `make` command. With `make run1` you can test the software on example files, `make ExecXPU` will compile the generated file and `make run2` will execute it.

To use Convertor on you own files, follow the syntax `./Convertor inputfile.c inputfile.mxif`.

The Makefile is easily readable and customizable.
