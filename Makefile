ROSE_INSTALL=/home/demo/opt/rose_inst
BOOST_INSTALL=/home/demo/opt/boost_1.47.0_inst

all: Convertor
 
Convertor: parser.o generator.o main.o
	g++ -g -o $@ $^ -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)/lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

parser.o: parser.cpp parser.h
	g++ -o $@ -Wall -Wfatal-errors -c $<
	
generator.o: generator.cpp generator.h
	g++ -o $@ -Wfatal-errors -c $< -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

main.o: main.cpp
	g++ -o $@ -g -Wfatal-errors -c $< -I $(BOOST_INSTALL)/include -I $(ROSE_INSTALL)/include/rose -L $(ROSE_INSTALL)/lib -l rose -L $(BOOST_INSTALL)/lib -l boost_iostreams

test: Convertor
	./test.sh g c x

clean:
	rm -rf Convertor *.o rose_* *.dot a.out *~ *.ti base_code.cpp ./mxifTests/*/output.* ./mxifTests/*/ExecXPU ./mxifTests/*/ExecSEQ *.log
