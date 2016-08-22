#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rose.h"
#include "generator.h"
#include "parser.h"

#define SIZE 50

using namespace std;
using namespace SageInterface;
using namespace SageBuilder;

void addElt(SgNode **list, SgNode *elt) {
	int i = 0;
	while (list[i] != NULL) {
		i++;
	}
	list[i] = elt;
}

/* Collection of all FunctionDeclaration, ForStatement and FunctionCall */
void visite (SgNode *sgn, char **codepath, SgNode **fd, SgNode **fs, SgNode **fc) {
	if (isSgFunctionDeclaration(sgn) != NULL) {
		addElt(fd, sgn);
	} else if (isSgForStatement(sgn) != NULL) {
		addElt(fs, sgn);
	} else if (isSgFunctionCallExp(sgn) != NULL) {
		SgFunctionDeclaration * fdd = isSgFunctionCallExp(sgn)->getAssociatedFunctionDeclaration();
		if (is_in(fdd, fd)) {
			addElt(fc, sgn);
		}
	}
	int a = (*sgn).get_numberOfTraversalSuccessors();

	if (isSgGlobal(sgn) != NULL) {
		for (int i = 0; i < a; i++) {
			SgNode *fils = (*sgn).get_traversalSuccessorByIndex(i);
			if (isSgNode(fils) != NULL) {
				Sg_File_Info *news = (*fils).get_file_info();
				std::string nodepath = (*news).get_raw_filename();
				if (strcmp(*codepath, nodepath.c_str()) == 0) {
					visite(fils, codepath, fd, fs, fc);
				}
			}
		}
	} else {
		for (int i = 0; i < a; i++) {
			SgNode *fils = (*sgn).get_traversalSuccessorByIndex(i);
			if (isSgNode(fils) != NULL) {
				visite(fils, codepath, fd, fs, fc);
			}
		}
	}
}

bool is_in (SgNode *elt, SgNode **list) {
	int i = -1;
	if (isSgFunctionDeclaration(elt)) {
		while (list[++i] != NULL) {
			if (strcmp(isSgFunctionDeclaration(elt)->get_name().str(), isSgFunctionDeclaration(list[i])->get_name().str()) == 0) {
				return true;
			}
		}
	}
	return false;
}

/* Get the relevant line of a task */
void get_lines(struct block * task, int * a) {
	char x1[4];
	strcpy(x1, task->child->bro->child->child->bro->bro->child->keyword);
	*a = atoi(x1);
}

/* Store relevant information of a task in a struct ftask */
void generate_ftask (struct block *rac, struct ftask **data, SgNode **fd, SgNode **fs) {
	struct block * tmp = rac->child;
	int i = -1;
	int j, k, ok, l;
	char func_name[SIZE];
	while (tmp->bro != NULL) {
		tmp = tmp->bro;
		if (strcmp(tmp->keyword, "task") == 0) {
			data[++i] = (struct ftask *)malloc(sizeof(struct ftask));
			data[i]->task = tmp;
			get_lines(data[i]->task, &j);
			
			ok = 0;
			k = -1;
			data[i]->lineNb = j;
			
			while (fd[++k] != NULL && ok == 0) {
				l = fd[k]->get_file_info()->get_line();
				if (l == j) {
					data[i]->core = fd[k];
					data[i]->isForLoop = false;
				}
			}
			if (ok == 0) {
				k = -1;
				while (fs[++k] != NULL && ok == 0) {
					l = fs[k]->get_file_info()->get_line();
					if (l == j) {
						data[i]->core = fs[k];
						data[i]->isForLoop = true;
					}
				}
			}
			
			if (strcmp(data[i]->task->child->bro->child->keyword, "chunk_of") == 0) {
				strcpy(func_name, data[i]->task->child->bro->child->child->bro->child->keyword);
			} else if (strcmp(data[i]->task->child->bro->child->keyword, "function") == 0) {
				strcpy(func_name, data[i]->task->child->bro->child->child->keyword);
			}
			if (strcmp(func_name, "main") == 0) {
				data[i]->isFromMain = true;
			} else {
				data[i]->isFromMain = false;
				
				k = -1;
				while (fd[++k] != NULL) {
					if (strcmp(isSgFunctionDeclaration(fd[k])->get_name().str(), func_name) == 0) {
						data[i]->src = isSgFunctionDeclaration(fd[k]);
						data[i]->rk = k;
						break;
					}
				}
			}
		}
	}
}

/* Make a copied SgNode "neutral" */
void file_info_setting (SgNode *rac) {
	if (isSgNode(rac)) {
		isSgLocatedNode(rac)->set_file_info(Sg_File_Info::generateDefaultFileInfoForTransformationNode());
		if (isSgLocatedNode(rac)->get_startOfConstruct()->isCompilerGenerated()) {
			isSgLocatedNode(rac)->get_file_info()->setCompilerGenerated();
		} else {
			isSgLocatedNode(rac)->get_file_info()->unsetCompilerGenerated();
		}
		size_t nb = rac->get_numberOfTraversalSuccessors();
		for (int i = 0; i < nb; i++) {
			file_info_setting(rac->get_traversalSuccessorByIndex(i));
		}
	}
}

/* Generate functions declarations for tasks */
void def_ftask (struct ftask *data, SgGlobal *sgg) {

	SgNode * sgn = data->core;
	
	if (data->isForLoop) {
		SgFunctionParameterList *params = buildFunctionParameterList();
		
		struct block *param = data->task->child->bro->child->child->bro->bro->bro;
		
		SgInitializedName * from = buildInitializedName("from", buildIntType());
		SgInitializedName * to = buildInitializedName("to", buildIntType());
		SgInitializedName * step = buildInitializedName("step", buildIntType());
		appendArg(params, from);
		appendArg(params, to);
		appendArg(params, step);
	
		while (strcmp(param->keyword, "input") == 0) {
			SgName var_name = param->child->keyword;
			SgType *type = (SgType *)malloc(sizeof(SgType));
			SgInitializedName * var = (SgInitializedName *)malloc(sizeof(SgInitializedName));
			switch ((param->child->bro->keyword)[1]) {
				case 'i':
					type = buildIntType();
					break;
				case 'c':
					type = buildCharType();
					break;
				case 'd':
					type = buildDoubleType();
					break;
				case 'f':
					type = buildFloatType();
					break;
				case 'l':
					type = buildLongType();
					break;
				default:
					break;
			}
		
			if ((param->child->bro->keyword)[strlen(param->child->bro->keyword) - 2] == '*') {
				var = buildInitializedName(var_name, buildPointerType(type));
			} else {
				var = buildInitializedName(var_name, type);
			}
		
			appendArg(params, var);
			param = param->bro;
		}
		char name[30];
		strcat(name, data->task->child->keyword);
		strcat(name, "_t");
		SgName func_name = name;
		SgFunctionDeclaration *func = buildDefiningFunctionDeclaration(func_name, buildIntType(), params, sgg);
		name[0] = '\0';
		prependStatement(func, sgg);
		data->def = func;

		SgNode *nd_copy = SgTreeCopy().copyAst(sgn);
		SgForStatement *for_copy = isSgForStatement(nd_copy);
		appendStatement(for_copy, func->get_definition()->get_body());
		file_info_setting(for_copy);
	
		appendStatement(buildReturnStmt(buildIntVal(0)), func->get_definition()->get_body());
		
	} else {
		char name[30];
		strcat(name, data->task->child->bro->child->child->keyword);
		strcat(name, "_t");
		SgFunctionParameterList *params = isSgFunctionParameterList(SgTreeCopy().copyAst(isSgFunctionDeclaration(sgn)->get_parameterList()));
		SgName func_name = name;
		SgFunctionDeclaration *func = buildDefiningFunctionDeclaration(func_name, buildIntType(), params, sgg);
		
		func->get_definition()->set_body(isSgBasicBlock(SgTreeCopy().copyAst(isSgFunctionDeclaration(sgn)->get_definition()->get_body())));
		
		file_info_setting(func);
		prependStatement(func, sgg);
		name[0] = '\0';
	}
}

/* Generate the main function */
void generateMain (SgGlobal * sgg1, SgGlobal * sgg2, struct ftask ** data, SgNode ** fc, struct block * rac) {

	SgBasicBlock * main1 = findMain(sgg1)->get_definition()->get_body(); // fichier source
	SgBasicBlock * main2 = findMain(sgg2)->get_definition()->get_body(); // fichier cible
	
	char output[5000];
	output[0] = '\0';
	
	/* Copy the first instructions */
	size_t nbStmt = main1->get_numberOfTraversalSuccessors();
	int i, j;
	int out;
	int xpu_done = 0;
	SgNode *lastStmt;
	
	for (i = 0; i < nbStmt; i++) {
		SgNode *stmt = SgTreeCopy().copyAst(main1->get_traversalSuccessorByIndex(i));
		out = 0;
		if (isSgForStatement(stmt)) {

			j = -1;
			while (data[++j] != NULL) {
				if (data[j]->isForLoop && data[j]->lineNb == stmt->get_startOfConstruct()->get_line()) {
					out = 1;
					break;
				}
			}
		} else if (isSgExprStatement(stmt)) {
			if (isSgFunctionCallExp(isSgExprStatement(stmt)->get_expression())) {
				
				j = -1;
				while (data[++j] != NULL) {
					if (data[j]->isFromMain == false && strcmp(data[j]->src->get_name().str(), isSgFunctionCallExp(isSgExprStatement(stmt)->get_expression())->getAssociatedFunctionDeclaration()->get_name().str()) == 0) {
						out = 1;
						break;
					}
				}
			}
		}
		
		if (out == 0) {
			file_info_setting(stmt);
			if (isSgVariableDeclaration(stmt)) {
				appendStatement(isSgVariableDeclaration(stmt), main2);
			} else {
				appendStatement(isSgStatement(stmt), main2);
			}
			lastStmt = stmt;
		} else if (out == 1 && xpu_done == 0) {
			geneXPU(output, rac, data, fc, main1, main2);
			attachArbitraryText(isSgLocatedNode(lastStmt), string(output), PreprocessingInfo::after);
			xpu_done = 1;
		}
	}
}

/* Generate the XPU part of the main function */
void geneXPU (char *output, struct block *rac, struct ftask **data, SgNode **fc, SgBasicBlock *m1, SgBasicBlock *m2) {
	int i = -1;
	int j, k, done;
	char buffer[100];
	buffer[0] = '\0';
	SgFunctionParameterList * pl1;
	SgFunctionParameterList * pl2;
	SgFunctionCallExp * call;
	
	bool alreadyCalled[SIZE] = { false };
	
	/* Declaration of tasks */
	while (data[++i] != NULL) {
		if (data[i]->isForLoop) {
		
			sprintf(buffer, "task %s(%s_t", data[i]->task->child->keyword, data[i]->task->child->keyword);
			strcat(output, buffer);
			strcat(output, ", 0, 0, 0");
			pl1 = data[i]->def->get_parameterList();
		
			if (data[i]->isFromMain) {
				for (j = 3; j < pl1->get_numberOfTraversalSuccessors(); j++) {
					sprintf(buffer, ", %s",
							isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))->get_name().str());
					strcat(output, buffer);
				}
			} else {
				pl2 = data[i]->src->get_parameterList();
				j = -1;
				while (fc[++j] != NULL) {
					/* looking for the function call */
					if (strcmp(data[i]->src->get_name().str(), isSgFunctionCallExp(fc[j])->getAssociatedFunctionDeclaration()->get_name().str()) == 0 && !alreadyCalled[j]) {
						call = isSgFunctionCallExp(fc[j]);
						alreadyCalled[j] = true;
						break;
					}	
				}
			
				j = 3;
			
				for (; j < pl1->get_numberOfTraversalSuccessors(); j++) {
					done = 0;
					for (k = 0; k < pl2->get_numberOfTraversalSuccessors(); k++) {
						if (strcmp(isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))->get_name().str(), isSgInitializedName(pl2->get_traversalSuccessorByIndex(k))->get_name().str()) == 0) {
							sprintf(buffer, ", %s", isSgVarRefExp(call->get_traversalSuccessorByIndex(1)->get_traversalSuccessorByIndex(k))->get_symbol()->get_name().str());
							
							strcat(output, buffer);
							done = 1;
							break;
						}
					}
					if (!done) {
						sprintf(buffer, ", %s", isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))
																	   ->get_name().str());
						if (!m1->symbol_exists(
								isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))->get_name().str()) &&
							!m2->symbol_exists(
								isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))->get_name().str())) {
							prependStatement(buildVariableDeclaration(isSgInitializedName(pl1->get_traversalSuccessorByIndex(j))->get_name(), buildIntType(), NULL, m2), m2);
						}
						strcat(output, buffer);
					}
				}
			}
		
			strcat(output, ");\n");
		} else {
			j = -1;
			while (fc[++j] != NULL) {
				/* looking for the function call */
				if (atoi(data[i]->task->child->bro->bro->bro->child->keyword) == isSgFunctionCallExp(fc[j])->get_startOfConstruct()->get_line()) {
					call = isSgFunctionCallExp(fc[j]);
					break;
				}	
			}
			SgExprListExp *pl = call->get_args();
			sprintf(buffer, "task %s(%s_t", data[i]->task->child->keyword, data[i]->task->child->bro->child->child->keyword);
			strcat(output, buffer);
			
			for (j = 0; j < pl->get_numberOfTraversalSuccessors(); j++) {
				sprintf(buffer, ", %s", pl->get_traversalSuccessorByIndex(j)->unparseToString().c_str());
			 	strcat(output, buffer);
			}
			strcat(output, ");\n");
		}
	}
	
	/* Declaration of the task_graph */
	struct block *tmp = rac->child;
	for (; tmp->bro != NULL; tmp = tmp->bro);
	
	struct ftask * ft = (struct ftask *)malloc(sizeof(struct ftask));
	
	for (; tmp->bro2 != NULL; tmp = tmp->bro2) {
		if (strcmp(tmp->keyword, "task_graph") == 0) {
			if (strcmp(tmp->child->bro->keyword, "parallel_for") == 0) {
				sprintf(buffer, "xpu::task_group *%s = new xpu::parallel_for(", tmp->child->keyword);
				strcat(output, buffer);
				
				/* looking for the for statement */
				i = -1;	
				while (data[++i] != NULL) {
					if (strcmp(tmp->child->bro->child->child->keyword, data[i]->task->child->keyword) == 0) {
						ft = data[i];
						break;
					}
				}
				
				sprintf(buffer, "%s, %s, 1, &%s);\n",
					ft->core->get_traversalSuccessorByIndex(0)
							->get_traversalSuccessorByIndex(0)
							->get_traversalSuccessorByIndex(0)
							->get_traversalSuccessorByIndex(1)
							->unparseToString().c_str(),
					ft->core->get_traversalSuccessorByIndex(1)
							->get_traversalSuccessorByIndex(0)
							->get_traversalSuccessorByIndex(1)
							->unparseToString().c_str(),
					ft->task->child->keyword);
											  
				strcat(output, buffer);
			} else if (strcmp(tmp->child->bro->keyword, "sequential") == 0 && tmp->child->bro->child->bro == NULL) {
				sprintf(buffer, "xpu::task_group *%s = %s;\n",
					tmp->child->keyword, tmp->child->bro->child->child->keyword);
				strcat(output, buffer);
			} else {
				sprintf(buffer, "xpu::task_group *%s = xpu::%s(",
					tmp->child->keyword, tmp->child->bro->keyword);
				strcat(output, buffer);
				
				struct block * tmp2 = tmp->child->bro->child;
				
				while (tmp2 != NULL) {
					if (strcmp(tmp2->keyword, "task") == 0) {
						strcat(output, "&");
					}
					strcat(output, tmp2->child->keyword);
					if (tmp2->bro != NULL) {
						strcat(output, ", ");
					}
					tmp2 = tmp2->bro;
				}
				strcat(output, ");\n");
			}
		}
	}
	
	strcat(output, "t.start();\n");
	sprintf(buffer, "%s->run();\n", tmp->bro->child->keyword);
	strcat(output, buffer);
	strcat(output, "t.stop();\n");
	strcat(output, "std::cout << \"[-] parallel execution: \" << t.elapsed() << std::endl;\n");
	strcat(output, "xpu::clean();\n");
}

/* Generate a dot file to show the parallel/sequential structure */
void geneDOT (struct block * rac) {
	FILE * f_dot = fopen("mxif.dot", "w");
	if (f_dot == NULL) {
		printf("Failure of the generation of the DOT file.\n");
		return;
	}
	
	fprintf(f_dot, "graph MXIF {\n");
	struct block *tmp, *tmp2;
	for (tmp = rac->child->bro; tmp != NULL; tmp = tmp->bro) {
		if (strcmp(tmp->keyword, "task_graph") == 0) {
			fprintf(f_dot, "\t%s [label=<<I>%s</I><BR/>%s>];\n",
						tmp->child->keyword, tmp->child->keyword, tmp->child->bro->keyword);
		} else if (strcmp(tmp->keyword, "task") == 0) {
			fprintf(f_dot, "\t%s [label=<<I>%s</I><BR/>task>];\n",
						tmp->child->keyword, tmp->child->keyword);
		}
	}
	for (tmp = rac->child->bro; tmp != NULL; tmp = tmp->bro) {
		if (strcmp(tmp->keyword, "task_graph") == 0) {
			for (tmp2 = tmp->child->bro->child; tmp2->bro != NULL; tmp2 = tmp2->bro);
			for (; tmp2 != NULL; tmp2 = tmp2->bro2) {
				fprintf(f_dot, "\t%s -- %s;\n", tmp->child->keyword, tmp2->child->keyword);
			}
		}
	}
	fprintf(f_dot, "}");
	fclose(f_dot);
}

int generateCode (int argc, char ** argv, SgProject * project, FILE * f_mxif) {
	/* PARSE MXIF */
	struct block * rac = (struct block *)malloc(sizeof(struct block));
	parseMXIF(f_mxif, rac);
	printf("Parse of the MXIF file succeeded.\n");
	
	/* VISITE */
	char * codepath = (char *)malloc(PATH_MAX);
	realpath(argv[1], codepath);
	
	SgGlobal *sgg = getFirstGlobalScope(project);
	
	SgNode **functDecl = (SgNode **)malloc(SIZE * sizeof(SgNode *));
	SgNode **forStmt = (SgNode **)malloc(SIZE * sizeof(SgNode *));
	SgNode **functCall = (SgNode **)malloc(SIZE * sizeof(SgNode *));
	bool *alreadyDef = (bool *)malloc(SIZE * sizeof(bool));
	
	int i;
	for (i = 0; i < SIZE; i++) {
		functDecl[i] = NULL;
		forStmt[i] = NULL;
		functCall[i] = NULL;
		alreadyDef[i] = false;
	}
	
	visite(sgg, &codepath, functDecl, forStmt, functCall);
	printf("Visit of the AST succeeded.\n");
	
	/* STRUCTURE FTASK */
	struct ftask *data[SIZE];
	for (i = 0; i < SIZE; i++) { data[i] = NULL; }
	generate_ftask(rac, data, functDecl, forStmt);
	printf("Extraction of the tasks succeeded.\n");
	
	/* OPENNING OF THE BASE CODE FOR THE OUTPUT FILE */
	char *arg_out = (char *)malloc(500 * sizeof(char));
	strcpy(arg_out, "./Convertor -I. -I/home/demo/xpu-0.1.5/ -L. -L/home/demo/xpu-0.1.5/.libs -lxpu-0.1.5 -lm -lpthread -lrt base_code.cpp");
	
	i = 0;
	char *argv2[SIZE];
	argv2[0] = strtok(arg_out, " ");
	while (++i < 10) {
		argv2[i] = strtok(NULL, " ");
	}
	
	SgProject *project2 = frontend(10, argv2);
	printf("Openning of the output file succeeded.\n");
	
	/* DECLARATION OF FTASKS */
	SgGlobal *sgg2 = getFirstGlobalScope(project2);
	i = -1;

	while (data[++i] != NULL) {
		if (data[i]->isForLoop || !alreadyDef[data[i]->rk]) {
			def_ftask(data[i], sgg2);
			if (!data[i]->isForLoop) {
				alreadyDef[data[i]->rk] = true;
			}
		}
	}

	generateDOT(*project2);

	generateMain(sgg, sgg2, data, functCall, rac);
	printf("Main function successfully generated.\n");
	
	AttachedPreprocessingInfoType save_buf;
	cutPreprocessingInfo(findFirstDefiningFunctionDecl(sgg), PreprocessingInfo::before, save_buf);
	cutPreprocessingInfo(findMain(sgg2), PreprocessingInfo::before, save_buf);
	pastePreprocessingInfo(findFirstDefiningFunctionDecl(sgg2), PreprocessingInfo::before, save_buf);
	
	geneDOT(rac);

	return backend(project2);
}
