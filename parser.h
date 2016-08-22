#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

struct block {
	struct block * dad;
	struct block * bro;
	struct block * bro2;
	struct block * child;
	char keyword[20];
};

void addChild(struct block *dad, struct block *child);
int getToken(FILE *f, char *tok);
void parseMXIF(FILE *f, struct block *rac);
void initBlock(struct block * b);

#endif
