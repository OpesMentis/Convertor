#!/bin/bash

declare -a FOLD=(00_singleFuncTwoCalls 01_singleLoopInMain 02_singleLoopSingleCall 03_singleLoopTwoCalls 04_multiLoopMultiCallSimple 05_multiLoopMultiCallSplit)

XPU_INSTALL=/home/demo/xpu-0.1.5/

if [ $# -gt 0 ]; then
	g="0"
	c="0"
	x="0"
	for i in "$@"
	do
		if [ $i = "g" ]; then
			g="1"
		elif [ $i = "c" ]; then
			c="1"
		elif [ $i = "x" ]; then
			x="1"
		else
			echo "Argument inconnu : $i."
		fi
	done
else
	g="1"
	c="1"
	x="1"
fi

if [ "$g" = "1" ]; then
	
	make

	for (( i=0; i<6; i++ ));
	do
		echo "Testcase number ${i} -- Code generation"
		FOLDi=./mxifTests/${FOLD[$i]}
		
		echo >> convertor.log
		echo "+++++++++++++++++++++++++++++++++++++++++" >> convertor.log
		echo "Code generation -- $(date +%d/%m/%Y) - $(date +%H:%M:%S) --" >> convertor.log
		echo "input files: ${FOLDi}/marked_test.c ${FOLDi}/marked_test.mxif" >> convertor.log
		echo "output file: ${FOLDi}/output.cpp" >> convertor.log
		echo "Logs of the generation currently written in convertor.log..."
		./Convertor -i ${FOLDi}/marked_test.c -m ${FOLDi}/marked_test.mxif -o ${FOLDi}/output.cpp -d ${FOLDi}/output.dot >> convertor.log
		echo "Testcase number ${i} -- Code generation over"
	done
fi

if [ "$c" = "1" ]; then
	for (( i=0; i<6; i++ ));
	do
		echo "Testcase number ${i} -- Code compilation"
		FOLDi=./mxifTests/${FOLD[$i]}
		
		g++ -O3 -Wfatal-errors -I. -I ${XPU_INSTALL} -c ${FOLDi}/output.cpp -o ${FOLDi}/output.o
		g++ -O3 -Wfatal-errors ${FOLDi}/output.o -o ${FOLDi}/ExecXPU -L. -L ${XPU_INSTALL}.libs -lxpu-0.1.5 -lm -lpthread -lrt
		g++ -O3 -Wfatal-errors ${FOLDi}/marked_test.c -o ${FOLDi}/ExecSEQ
	done
fi

if [ "$x" = "1" ]; then
	for (( i=0; i<6; i++ ));
	do
		echo "Testcase number ${i} -- Code execution & comparison"
		FOLDi=./mxifTests/${FOLD[$i]}
	
		${FOLDi}/ExecXPU
		${FOLDi}/ExecSEQ
	done
fi
