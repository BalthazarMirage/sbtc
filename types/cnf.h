#ifndef CNF_H
#define CNF_H

#include <stddef.h>

typedef struct {
	size_t num_vars, num_clauses;
	size_t * clauses;
	int * datas;
} Cnf;


size_t cnf_get_num_vars (Cnf * cnf);
size_t cnf_get_num_clauses (Cnf * cnf);
size_t cnf_get_size (Cnf * cnf);

size_t cnf_get_clause_size (Cnf * cnf, size_t clause);
int * cnf_get_clause_begin (Cnf * cnf, size_t clause);
int * cnf_get_clause_end (Cnf * cnf, size_t clause);

#endif
