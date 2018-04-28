#include "cnf.h"

size_t cnf_get_num_vars (Cnf * cnf) { return cnf->num_vars; }

size_t cnf_get_num_clauses (Cnf * cnf) { return cnf->num_clauses; }

size_t cnf_get_size (Cnf * cnf) { return cnf->clauses[cnf->num_clauses]; }

size_t cnf_get_clause_size (Cnf * cnf, size_t clause) {
	return cnf->clauses[clause+1] - cnf->clauses[clause];
}

int * cnf_get_clause_begin (Cnf * cnf, size_t clause) {
	return cnf->datas + cnf->clauses[clause];
}

int * cnf_get_clause_end (Cnf * cnf, size_t clause) {
	return cnf->datas + cnf->clauses[clause+1];
}
