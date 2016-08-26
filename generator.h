#ifndef GENERATOR_H
#define GENERATOR_H

struct ftask {
	struct block * task;
	bool isForLoop; // task represents a for loop
	bool isFromMain; // task is extracted from main function
	int lineNb; // relevant line of the task
	int rk; // rank of the function declaration in SgFunctionDeclaration **fd
	SgFunctionDeclaration *src; // function declaration where task come from
	SgNode *core; // smallest container of the task (function declaration or for loop)
	SgFunctionDeclaration *def; // function declaration of the task in the output file
};

void addElt (SgNode **list, SgNode *elt);
void visite (SgNode *sgn, char **codepath, SgNode **fd, SgNode **fs, SgNode **fc);
int generateCode (SgProject *project, FILE * f_mxif, char **argv, char *cpp_output, char *dot_output);
void generate_ftask (struct block *rac, struct ftask **data, SgNode **fd, SgNode **fs);
void get_lines (struct block *task, int *a);
bool is_in (SgNode *elt, SgNode **list);
void file_info_setting (SgNode *rac);
void def_ftask (struct ftask *data, SgGlobal *sgg);
void geneXPU (char *output, struct block *rac, struct ftask **data, SgNode **fc, SgBasicBlock *m1, SgBasicBlock *m2);
void generateMain (SgGlobal *sgg1, SgGlobal *sgg2, struct ftask **data, SgNode **fc, struct block *rac);
void gene_base_code ();
void geneDOT (struct block * rac, char *dot_output);

#endif
