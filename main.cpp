#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rose.h"
#include "generator.h"

int main (int argc, char ** argv) {
	if (argc < 3) {
		printf("Missing file(s) in input.\n");
		printf("Usage: './Convertor file.c file.mxif'.\n");
		exit(0);
	}

	FILE * f_mxif = fopen(argv[2], "r");
	if (f_mxif == NULL) {
		printf("Failure in the openning of the MXIF file.\n");
		exit(0);
	}
	argv[2] = NULL;
	SgProject *project = frontend(2, argv);
	if (project == NULL) {
		printf("Failure in the openning of the C file.\n");
		exit(0);
	}
	generateCode(argc, argv, project, f_mxif);
	fclose(f_mxif);
	
	return 0;
}
