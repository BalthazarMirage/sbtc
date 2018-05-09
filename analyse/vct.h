#ifndef VCT_H
#define VCT_H

// Variable Covering Table

#include "../cnf/cnf.h"

typedef struct {
	int num_vars;
	int * vars;
	int * datas;
}Vct;

Vct * vct_create (Cnf * cnf, int num_membs, int * membs);
void  vct_destroy (Vct * vct);

int vct_size (Vct * vct, int var);
int * vct_begin (Vct * vct, int var);
int * vct_end (Vct * vct, int var);

int vct_clause (Vct * vct, int var, int cls);
#endif
