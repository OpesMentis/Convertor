#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <getopt.h>
#include "rose.h"
#include "generator.h"

int main (int argc, char ** argv) {

	char c_input[100]; c_input[0] = '\0';
	char mxif_input[100]; mxif_input[0] = '\0';
	
	char cpp_output[100]; cpp_output[0] = '\0';
	strcpy(cpp_output, "output_file.cpp");
	
	char dot_output[100]; dot_output[0] = '\0';
	strcpy(dot_output, "task.dot");
	
	int opt = 0;
	
	static struct option long_options[] = {
		{ "input",  required_argument, 0, 'i' },
		{ "mxif",   required_argument, 0, 'm' },
		{ "output", required_argument, 0, 'o' },
		{ "dot",	required_argument, 0, 'd' },
		{ 0,		0,				   0, 0 }
	};
	
	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "i:m:o:d:", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'i': strcpy(c_input, optarg);
				break;
			case 'm': strcpy(mxif_input, optarg);
				break;
			case 'o': strcpy(cpp_output, optarg);
				break;
			case 'd': strcpy(dot_output, optarg);
				break;
			default: printf("Usage: ./Convertor -i input.c -m input.mxif [-o output.cpp] [-d tasks.dot]\n");
				exit(EXIT_FAILURE);
		}
	}
	
	if (c_input[0] == '\0' || mxif_input[0] == '\0') {
		printf("Usage: ./Convertor -i input.c -m input.mxif [-o output.cpp] [-d tasks.dot]\n");
		exit(EXIT_FAILURE);
	}

	FILE * f_mxif = fopen(mxif_input, "r");
	if (f_mxif == NULL) {
		printf("Failure in the openning of the MXIF file.\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(argv[1], c_input);
	SgProject *project = frontend(2, argv);
	if (project == NULL) {
		printf("Failure in the openning of the C file.\n");
		exit(EXIT_FAILURE);
	}
	generateCode(project, f_mxif, argv, cpp_output, dot_output);
	fclose(f_mxif);
	
	return 0;
}
