#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

void addChild(struct block *dad, struct block *child) {
	if (dad->child == NULL) {
		dad->child = child;
	} else {
		struct block * tmp = dad->child;
		while (tmp->bro != NULL) {
			tmp = tmp->bro;
		}
		tmp->bro = child;
		child->bro2 = tmp;
	}
	child->dad = dad;
}

int getToken(FILE *f, char *tok) {
	char ch;
	while (strchr(" \t\n", (int)(ch = fgetc(f))) != NULL);
	if (ch == EOF) {
		return -1;
	}
	if (strchr("()", (int)ch) != NULL) {
		tok[0] = ch;
		tok[1] = '\0';
	} else if (strchr("\"", (int)ch) != NULL) {
		tok[0] = '\0';
		while ((ch = fgetc(f)) != '\"') {
			strncat(tok, &ch, 1);
		}
		strncat(tok, "\0", 1);
	} else {
		tok[0] = ch;
		tok[1] = '\0';
		while (strchr(" \t\n()", (int)(ch = fgetc(f))) == NULL) {
			strncat(tok, &ch, 1);
		}
		strncat(tok, "\0", 1);
		fseek(f, -1, SEEK_CUR);
	}
	return 1;
}

void initBlock(struct block * b) {
	b->dad = NULL;
	b->bro = NULL;
	b->bro2 = NULL;
	b->child = NULL;
	(b->keyword)[0] = '\0';
}

void parseMXIF (FILE *f, struct block *rac) {
	initBlock(rac);
	char tok[20];
	struct block **stack = (struct block **)malloc(sizeof(struct block *) * 100);
	int sp = -1; // stack pointer
	int kw = -1; // keyword expected if > 0
	while (getToken(f, tok) > 0) {
		if (kw > 0) {
			strcpy(stack[sp]->keyword, tok);
			kw = -1;
		} else if (strcmp(tok, "(") == 0) {
			sp++;
			if (sp == 0) {
				stack[sp] = rac;
			} else {
				struct block * b = (struct block *)malloc(sizeof(struct block));
				initBlock(b);
				stack[sp] = b;
				addChild(stack[sp-1], stack[sp]);
			}
			kw = 1;
		} else if (strcmp(tok, ")") == 0) {
			sp--;
		} else {
			struct block * b = (struct block *)malloc(sizeof(struct block));
			initBlock(b);
			strcpy(b->keyword, tok);
			addChild(stack[sp], b);
		}
	}
}

/*
int main(int argc, char *argv[]) {
	FILE *f_mxif = fopen("test2.mxif", "r");
	
	if (f_mxif == NULL) {
		printf("Impossible d'ouvrir le fichier.");
		return 0;
	}
	
	struct block *rac = (struct block *)malloc(sizeof(struct block));
	
	parseMXIF(f_mxif, rac);
	printf("%s\n", rac->child->bro->bro2->bro->keyword);
	
	fclose(f_mxif);
	
	return 1;
}*/
