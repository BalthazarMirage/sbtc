#include "vct.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Vct * vct_create (Cnf * cnf, int num_membs, int * membs) {
	Vct * vct = malloc (
		sizeof (Vct) +
		sizeof (int) * (cnf_num_vars(cnf)+1) +
		sizeof (int) * (cnf_num_litterals(cnf))
	);
	vct->num_vars = cnf_num_vars(cnf);
	vct->vars = (int *) (vct + 1);
	vct->datas = (int *) (vct->vars + vct->num_vars + 1);
	
	memset (vct->vars, 0, sizeof(size_t) * (vct->num_vars + 1));
	
	for (int i = 0; i < num_membs; i++) {		
		for (int * v = cnf_begin(cnf, membs[i]); v != cnf_end(cnf, membs[i]); v++) {
			vct->vars[abs(*v)+1]++;
		}
	}
	
	for (int i = 0; i <= vct->num_vars; i++)
		vct->vars[i] += vct->vars[i-1];
	
	for (int i = 0; i < num_membs; i++) {
		for (int * v = cnf_begin(cnf, membs[i]); v != cnf_end(cnf, membs[i]); v++) {
			int var = vct->vars[abs(*v)]++;
			vct->datas[var] = ((*v) > 0) ? membs[i]+1 : -membs[i]-1;
		}
	}
		
	vct = realloc (vct,
		sizeof(Vct) +
		sizeof(int) * (vct->num_vars+1) +
		sizeof(int) * (vct->vars[vct->num_vars])
	);
	
	return vct;
}

void vct_destroy(Vct * vct) {
	free (vct);
}

int vct_size (Vct * vct, int var) {
	return vct->vars[abs(var)] - vct->vars[abs(var)-1];
}

int * vct_begin (Vct * vct, int var) {
	return vct->datas + vct->vars[abs(var)-1];
}

int * vct_end (Vct * vct, int var) {
	return vct->datas + vct->vars[abs(var)];
}

int vct_clause (Vct * vct, int var, int cl) {
	return vct_begin(vct, var)[cl];
}
