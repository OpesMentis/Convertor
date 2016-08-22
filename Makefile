INPUTCODE=mxifTests/04_multiLoopMultiCallSimple/marked_test.c
INPUTMXIF=mxifTests/04_multiLoopMultiCallSimple/marked_test.mxif
ROSE_INSTALL=/home/demo/opt/rose_inst
BOOST_INSTALL=/home/demo/opt/boost_1.47.0_inst
XPU_INSTALL=/home/demo/xpu-0.1.5/

all: Convertor
 
Convertor: parser.o generator.o main.o
	g++ -g -o Convertor main.o parser.o generator.o -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)/lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

parser.o: parser.cpp
	g++ -o parser.o -Wall -Wfatal-errors -c parser.cpp
	
generator.o: generator.cpp
	g++ -o generator.o -Wfatal-errors -c generator.cpp -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

main.o: main.cpp
	g++ -o main.o -g -Wfatal-errors -c main.cpp -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)/lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

run1: Convertor
	./Convertor $(INPUTCODE) $(INPUTMXIF)
	
rose_base_code.o: rose_base_code.cpp
	g++ -O3 -Wfatal-errors -I. -I $(XPU_INSTALL) -c rose_base_code.cpp -o rose_base_code.o

ExecXPU: rose_base_code.o
	g++ -O3 -Wfatal-errors rose_base_code.o -o ExecXPU -L. -L $(XPU_INSTALL).libs -lxpu-0.1.5 -lm -lpthread -lrt

run2: ExecXPU
	./ExecXPU

clean:
	rm -rf Convertor ExecXPU *.o rose_* *.dot a.out *~ *.ti

