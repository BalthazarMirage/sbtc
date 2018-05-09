#ifndef CNF_H
#define CNF_H

#include <stddef.h>

typedef struct {
	int num_vars, num_clauses;
	int * clauses;
	int * datas;
} Cnf;


Cnf * cnf_load (const char * filename);
void cnf_save (Cnf * cnf, const char * filename);
void  cnf_destroy (Cnf * cnf);

int cnf_num_vars (Cnf * cnf);
int cnf_num_clauses (Cnf * cnf);
int cnf_num_litterals (Cnf * cnf);

int cnf_size (Cnf * cnf, int clause);
int * cnf_begin (Cnf * cnf, int clause);
int * cnf_end (Cnf * cnf, int clause);

int cnf_litteral (Cnf * cnf, int clause, int litteral);

#endif
