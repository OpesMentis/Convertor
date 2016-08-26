# Convertor

Code written in the framework of a three-month internship in TU Delft about the parallelisation of C programs. This Linux software converts a C code and an MXIF file into a parallelised C++ code using the *XPU* library. A dot file is also generated with a graph showing the parallel/sequential structure.

**Requirements**

This software requires at least the *Rose compiler* library to execute. MXIF files are generated thanks to the *MCPROF* application.To compile and execute the generated code, the *XPU* library is necessary. These tools are available here:

- [*Rose compiler*](http://rosecompiler.org/) (the use of a virtual machine is the easiest and recommanded way)
- [*MCPROF*](https://bitbucket.org/imranashraf/mcprof)
- [*XPU*](http://www.xpu-project.net/en/index.php)

**How to use Convertor**

You can compile the software thanks to the classic `make` command.

With `make test` or by calling directly the bash script `test.sh` you can test Convertor on the test cases in the `testMxif` folder. According to the arguments given to `test.sh` more or less will be done.

- `test.sh g` will generate the C++/XPU files.
- `test.sh c` will compile the C++/XPU files.
- `test.sh x` will execute the generated C++/XPU files and the original C files.

All the arguments can be combined, seperated by spaces. If no argument is given, everything is done by default. Information about the generation is written in a `convertor.log` file.

To use Convertor on your own files, follow the syntax `./Convertor -i inputfile.c -m inputfile.mxif -o outputfile.cpp -d outputfile.dot`. Only the `-i` and `-d` arguments are mandatory, default names will be given to the output files if nothing is specified. The order of parameters does not matter.

`Makefile` and the script `test.sh` are easily readable and customizable.
