#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rose.h"
#include "generator.h"

int main (int argc, char ** argv) {
	FILE * f_mxif = fopen(argv[2], "r");
	if (f_mxif == NULL) {
		exit(0);
	}
	argv[2] = NULL;
	SgProject * project = frontend(2, argv);
	generateCode(argc, argv, project, f_mxif);
	fclose(f_mxif);
	
	return 0;
}
